
#define MY_VERSION_ID 0x101
#define MY_FUNCTION_MACRO(x) ((x) + 1)
#define MY_FUNCTION_STRING "v1.0.1"

typedef int my_int_t;
typedef unsigned int my_uint_t;
typedef char my_char_t;
typedef unsigned char my_uchar_t;
typedef short my_short_t;
typedef unsigned short my_ushort_t;
typedef float my_float_t;

typedef struct my_struct_t {
	int x;
	int y;
	int z;
} my_struct_t;

typedef struct my_other_struct_t {
	my_int_t a;
	my_uint_t b;
	my_char_t c;
	my_uchar_t d;
	my_short_t e;
	my_ushort_t f;
	my_float_t g;
} my_other_struct_t;

int my_function(int a, int b, my_float_t c);
int my_other_function(char *ptr);
