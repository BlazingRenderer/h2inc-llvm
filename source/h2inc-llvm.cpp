
#include <iostream>
#include <fstream>
#include <string>
#include <clang-c/Index.h>
#include "h2inc-llvm.h"

/*
 * globals
 */
CXIndex           c_index;
CXTranslationUnit c_unit;
CXCursor          c_cursor;
std::ofstream     dstfile;

/*
 * cxstring -> ostream helper
 */
std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

/*
 * cursor visitor function
 */
static enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXCursorKind kind   = clang_getCursorKind(cursor);
    CXEvalResult result = clang_Cursor_Evaluate(cursor);

    if(clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0) {
        return CXChildVisit_Continue;
    }

    switch(kind) {
        /*
         * macro definition
         */
        case CXCursor_MacroDefinition: {
            CXToken     *tokens     = nullptr;
            unsigned int num_tokens = 0;
            dstfile << clang_getCursorSpelling(cursor) << " EQU ";
            CXSourceRange range = clang_getCursorExtent(cursor);
            clang_tokenize(c_unit, range, &tokens, &num_tokens);
            for(int i = 0; i < num_tokens; i++) {
                if(i == 0)
                    continue;
                CXString    tokentext = clang_getTokenSpelling(c_unit, tokens[i]);
                CXTokenKind tokenkind = clang_getTokenKind(tokens[i]);
                if(tokenkind == CXToken_Literal) {
                    dstfile << tokentext << std::endl;
                }
            }
            clang_disposeTokens(c_unit, tokens, num_tokens);
            break;
        }

        /*
         * typedef declaration
         */
        case CXCursor_TypedefDecl: {
            CXType      type = clang_getTypedefDeclUnderlyingType(cursor);
            std::string typestr;

            switch(type.kind) {
                case CXType_Long:
                case CXType_Int:
                case CXType_Char32:
                    typestr = "SDWORD";
                    break;
                case CXType_ULong:
                case CXType_UInt:
                    typestr = "DWORD";
                    break;
                case CXType_Short:
                case CXType_Char16:
                    typestr = "SWORD";
                    break;
                case CXType_UShort:
                    typestr = "WORD";
                    break;
                case CXType_SChar:
                case CXType_Char_S:
                    typestr = "SBYTE";
                    break;
                case CXType_UChar:
                case CXType_Char_U:
                    typestr = "BYTE";
                    break;
                case CXType_Float:
                    typestr = "REAL4";
                    break;
                default:
                    std::cerr << "invalid type in typedef decl" << std::endl;
                    break;
            }

            if(!typestr.empty()) {
                dstfile << clang_getCursorSpelling(cursor) << " TYPEDEF " << typestr << std::endl;
            }

            break;
        }
    }

    clang_EvalResult_dispose(result);

    std::cout << "Cursor '" << clang_getCursorSpelling(cursor) << "' of kind '"
              << clang_getCursorKindSpelling(clang_getCursorKind(cursor)) << "'\n";

    return CXChildVisit_Recurse;
}

/*
 * process func
 */
bool h2inc(const char *src, const char *dst)
{
    /*
     * open source file
     */
    c_index = clang_createIndex(0, 0);
    c_unit = clang_parseTranslationUnit(c_index, src, nullptr, 0, nullptr, 0, CXTranslationUnit_DetailedPreprocessingRecord);
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
    c_cursor = clang_getTranslationUnitCursor(c_unit);
    clang_visitChildren(c_cursor, visitor, nullptr);

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
    return h2inc(argv[1], argv[2]) ? 0 : 1;
}
