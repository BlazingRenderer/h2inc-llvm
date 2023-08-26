
#include <iostream>
#include <fstream>
#include <string>
#include <clang-c/Index.h>
#include "h2inc-llvm.h"

/*
 * default facilities
 */

extern h2inc::facility *masm_facility;

struct visitor_client_data {
    std::ofstream    file;
    h2inc::facility *facility;
};

/*
 * cursor visitor function
 */
static enum CXChildVisitResult new_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXCursorKind     kind     = clang_getCursorKind(cursor);
    std::ofstream   *file     = &((visitor_client_data *)client_data)->file;
    h2inc::facility *facility = ((visitor_client_data *)client_data)->facility;

    if(kind == CXCursor_TypedefDecl) {
        facility->write_typedef(file, clang_getCString(clang_getCursorSpelling(cursor)),
                                clang_getCString(clang_getTypeSpelling(clang_getTypedefDeclUnderlyingType(cursor))));
    }

    return CXChildVisit_Continue;
}

/*
 * process (advanced)
 */
bool h2inc::run_advanced(std::string src, std::string dst, h2inc::facility *f)
{
    visitor_client_data client_data;

    CXIndex           index = clang_createIndex(0, 0);
    CXTranslationUnit unit  = clang_parseTranslationUnit(index, src.c_str(), nullptr, 0, nullptr, 0,
                                                         CXTranslationUnit_DetailedPreprocessingRecord);

    client_data.file.open(dst, std::ofstream::out);
    client_data.facility = f;
    CXCursor cursor      = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, new_visitor, &client_data);
    client_data.file.close();

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return true;
}

/*
 * globals
 */
static CXIndex           c_index;
static CXTranslationUnit c_unit;
static CXCursor          c_cursor;
static std::ofstream     dstfile;
static std::string       last_struct_identifier;
static int               num_protos = 0;
static std::string       last_function_name;
static int               num_args    = 0;
static int               current_arg = 0;

/*
 * cxstring -> ostream helper
 */
std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

static std::string typestring_to_masm(std::string s)
{
    std::string res = "";

    /*
     * check if its a ptr
     */
    if(s.back() == '*') {
        res = "PTR ";
        s.pop_back();
        s.pop_back();
    }

    if(s == "long" || s == "int" || s == "signed long" || s == "signed int")
        res.append("SDWORD");
    else if(s == "unsigned long" || s == "unsigned int")
        res.append("DWORD");
    else if(s == "short" || s == "signed short")
        res.append("SWORD");
    else if(s == "unsigned short")
        res.append("WORD");
    else if(s == "char" || s == "signed char")
        res.append("SBYTE");
    else if(s == "unsigned char")
        res.append("BYTE");
    else if(s == "float")
        res.append("REAL4");
    else
        res.append(s);

    return res;
}

static std::string type_to_masm(CXType type)
{
    switch(type.kind) {
        case CXType_Long:
        case CXType_Int:
        case CXType_Char32:
            return "SDWORD";
        case CXType_ULong:
        case CXType_UInt:
            return "DWORD";
        case CXType_Short:
        case CXType_Char16:
            return "SWORD";
        case CXType_UShort:
            return "WORD";
        case CXType_SChar:
        case CXType_Char_S:
            return "SBYTE";
        case CXType_UChar:
        case CXType_Char_U:
            return "BYTE";
        case CXType_Float:
            return "REAL4";
        default:
            return typestring_to_masm(clang_getCString(clang_getTypeSpelling(type)));
    }

    return "INVALID TYPE";
}

static std::string cursor_to_masm(CXCursor cursor)
{
    return type_to_masm(clang_getCursorType(cursor));
}

/*
 * cursor visitor function
 */
static enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXCursorKind kind = clang_getCursorKind(cursor);

    /*
     * so we don't get gcc/clang headers
     */
    if(clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0) {
        return CXChildVisit_Continue;
    }

    /*
     * switch based on cursor kind
     */
    switch(kind) {

        /*
         * function prototypes
         */
        case CXCursor_FunctionDecl: {
            /*
             * add prototype macro
             */
            dstfile << "@proto_" << num_protos << " TYPEDEF PROTO C";
            last_function_name = clang_getCString(clang_getCursorSpelling(cursor));
            num_protos += 1;
            num_args = clang_getNumArgTypes(clang_getCursorType(cursor));
            return CXChildVisit_Recurse;
        }

        /*
         * function prototype parameter
         */
        case CXCursor_ParmDecl: {
            dstfile << " :" << cursor_to_masm(cursor);
            current_arg += 1;
            if(current_arg >= num_args) {
                current_arg = 0;
                num_args    = 0;
                dstfile << std::endl
                        << last_function_name << " PROTO @proto_" << num_protos - 1 << std::endl
                        << std::endl;
                break;
            } else {
                dstfile << ",";
            }
            break;
        }

        /*
         * struct definitions
         */
        case CXCursor_StructDecl: {
            last_struct_identifier = clang_getCString(clang_getCursorSpelling(cursor));
            dstfile << last_struct_identifier << " STRUCT 4t" << std::endl;
            return CXChildVisit_Recurse;
        }

        /*
         * struct member
         */
        case CXCursor_FieldDecl: {
            dstfile << clang_getCursorSpelling(cursor) << " " << cursor_to_masm(cursor) << " ?" << std::endl;
            break;
        }

        /*
         * macro definition
         */
        case CXCursor_MacroDefinition: {
            CXToken     *tokens     = nullptr;
            unsigned int num_tokens = 0;
            if(clang_Cursor_isMacroFunctionLike(cursor))
                break;
            dstfile << clang_getCursorSpelling(cursor) << " EQU ";
            CXSourceRange range = clang_getCursorExtent(cursor);
            clang_tokenize(c_unit, range, &tokens, &num_tokens);
            for(int i = 0; i < num_tokens; i++) {
                if(i == 0)
                    continue;
                CXString    tokentext = clang_getTokenSpelling(c_unit, tokens[i]);
                CXTokenKind tokenkind = clang_getTokenKind(tokens[i]);
                if(tokenkind == CXToken_Literal) {
                    dstfile << tokentext << std::endl << std::endl;
                }
            }
            clang_disposeTokens(c_unit, tokens, num_tokens);
            break;
        }

        /*
         * typedef declaration
         */
        case CXCursor_TypedefDecl: {
            CXType      type    = clang_getTypedefDeclUnderlyingType(cursor);
            std::string typestr = "";

            switch(type.kind) {
                case CXType_Elaborated:
                    dstfile << last_struct_identifier << " ENDS" << std::endl << std::endl;
                    break;
                default:
                    typestr = type_to_masm(type);
                    break;
            }

            if(!typestr.empty() && typestr != "INVALID TYPE") {
                dstfile << clang_getCursorSpelling(cursor) << " TYPEDEF " << typestr << std::endl << std::endl;
            }

            break;
        }
    }

    std::cout << "Cursor '" << clang_getCursorSpelling(cursor) << "' of kind '"
              << clang_getCursorKindSpelling(clang_getCursorKind(cursor)) << "'\n";

    return CXChildVisit_Continue;
}

/*
 * process func
 */
bool h2inc::run(std::string src, std::string dst)
{
    /*
     * open source file
     */
    c_index = clang_createIndex(0, 0);
    c_unit  = clang_parseTranslationUnit(c_index, src.c_str(), nullptr, 0, nullptr, 0,
                                         CXTranslationUnit_DetailedPreprocessingRecord);
    if(c_unit == nullptr) {
        std::cerr << "failed to process " << src << std::endl;
        return false;
    }

    std::cout << "successfully processed " << src << std::endl;

    /*
     * open destination file
     */
    dstfile.open(dst, std::ofstream::out);
    if(!dstfile.is_open()) {
        std::cerr << "failed to open destination file " << dst << std::endl;
        return false;
    }

    /*
     * run through children
     */
    dstfile << "; Begin of file " << src << std::endl << std::endl;
    c_cursor = clang_getTranslationUnitCursor(c_unit);
    clang_visitChildren(c_cursor, visitor, nullptr);
    dstfile << "; End of file " << src << std::endl;

    /*
     * clean up memory
     */
    dstfile.close();
    clang_disposeTranslationUnit(c_unit);
    clang_disposeIndex(c_index);

    /*
     * return success
     */
    return true;
}

/*
 * main
 */
int main(int argc, char *argv[])
{
    h2inc::run(argv[1], argv[2]);
    return 0;
}
