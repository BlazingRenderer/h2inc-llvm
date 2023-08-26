
#include <string>
#include <vector>

namespace h2inc
{

struct parameter {
    /* type name string */
    std::string type;

    /* name string */
    std::string name;

    /* array size (if 0, it's not an array) */
    int array_size;

    /* number of pointer abstractions (if 0, it's not a pointer) */
    int pointer_size;
};

struct facility {

    /* write typedef */
    virtual void write_typedef(std::ofstream *file, std::string name, std::string type) = 0;

    /* write struct typedef */
    virtual void write_struct_typedef(std::ofstream *file, std::string name, std::vector<parameter>& members) = 0;

    /* write macro */
    virtual void write_macro(std::ofstream *file, std::string name, std::string value) = 0;

    /* write function prototype */
    virtual void write_function_prototype(std::ofstream *file, std::string name, std::vector<parameter>& parameters) = 0;
};

bool run_advanced(std::string src, std::string dst, facility *f);
bool run(std::string src, std::string dst);

} // namespace h2inc
