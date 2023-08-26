
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <clang-c/Index.h>
#include "h2inc-llvm.h"

/*
 *
 * masm facility (default)
 *
 */
struct facility_masm : public h2inc::facility {

    int num_protos = 0;

    /*
     * convert type string to masm type
     */
    std::string type_to_masm(std::string type)
    {
        std::string result;

        /*
         * check if its a pointer
         */
        if(type.back() == '*') {
            result = "PTR ";
            type.pop_back();
            type.pop_back();
        }

        /*
         * check based on type
         */
        if(type == "long" || type == "signed long")
            result.append("SQWORD");
        else if(type == "unsigned long")
            result.append("QWORD");
        else if(type == "int" || type == "signed int")
            result.append("SDWORD");
        else if(type == "unsigned int")
            result.append("DWORD");
        else if(type == "short" || type == "signed short")
            result.append("SWORD");
        else if(type == "unsigned short")
            result.append("WORD");
        else if(type == "char" || type == "signed char")
            result.append("SBYTE");
        else if(type == "unsigned char")
            result.append("BYTE");
        else if(type == "float")
            result.append("REAL4");
        else if(type == "double")
            result.append("REAL8");
        else if(type == "long double")
            result.append("REAL10");
        else
            result.append(type);

        return result;
    }

    /*
     * write typedef
     */
    void write_typedef(std::ofstream *file, std::string name, std::string type)
    {
        (*file) << name << " TYPEDEF " << type_to_masm(type) << std::endl;
    }

    /*
     * write struct typedef
     */
    void write_struct_typedef(std::ofstream *file, std::string name, std::vector<h2inc::parameter>& members)
    {
        (*file) << name << " STRUCT 4t" << std::endl;
        for(auto& member : members) {
            (*file) << member.name << " " << type_to_masm(member.type) << " ?" << std::endl;
        }
        (*file) << name << " ENDS" << std::endl;
    }

    /*
     * write macro
     */
    void write_macro(std::ofstream *file, std::string name, std::string value)
    {
        (*file) << name << " EQU " << value << std::endl;
    }

    /*
     * write function prototype
     */
    void write_function_prototype(std::ofstream *file, std::string name, std::vector<h2inc::parameter>& parameters)
    {
        (*file) << "@proto_" << num_protos << " TYPEDEF PROTO C ";
        for(int i = 0; i < parameters.size(); i++) {
            if(i < parameters.size() - 1)
                (*file) << ": " << type_to_masm(parameters[i].type) << ", ";
            else
                (*file) << ": " << type_to_masm(parameters[i].type) << std::endl;
        }
        (*file) << name << " PROTO @proto_" << num_protos << std::endl;
        num_protos += 1;
    }

} masm_facility_private;

h2inc::facility *masm_facility = &masm_facility_private;
