
#define MY_VERSION_ID 0x1001

typedef int            my_int_t;
typedef unsigned int   my_uint_t;
typedef char           my_char_t;
typedef unsigned char  my_uchar_t;
typedef short          my_short_t;
typedef unsigned short my_ushort_t;
typedef float          my_float_t;

typedef struct my_struct_t {
    int x;
    int y;
    int z;
} my_struct_t;

typedef struct my_other_struct_t {
    my_int_t    a;
    my_uint_t   b;
    my_char_t   c;
    my_uchar_t  d;
    my_short_t  e;
    my_ushort_t f;
    my_float_t  g;
} my_other_struct_t;
