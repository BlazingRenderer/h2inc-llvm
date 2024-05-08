
option expr32
option casemap:none

; Begin of file test.h
MY_VERSION_ID		EQU		000001001h
my_int_t		TYPEDEF		SDWORD

my_uint_t		TYPEDEF		DWORD

my_char_t		TYPEDEF		SBYTE

my_uchar_t		TYPEDEF		BYTE

my_short_t		TYPEDEF		SWORD

my_ushort_t		TYPEDEF		WORD

my_float_t		TYPEDEF		REAL4

my_struct_t		STRUCT 4t
x		SDWORD		?
y		SDWORD		?
z		SDWORD		?
my_struct_t		ENDS

my_other_struct_t		STRUCT 4t
a		my_int_t		?
b		my_uint_t		?
c		my_char_t		?
d		my_uchar_t		?
e		my_short_t		?
f		my_ushort_t		?
g		my_float_t		?
my_other_struct_t		ENDS

@proto_0		TYPEDEF		PROTO STDCALL :SDWORD, :SDWORD, :my_float_t
my_function		PROTO		@proto_0

@proto_1		TYPEDEF		PROTO STDCALL :PTR SBYTE
my_other_function		PROTO		@proto_1

; End of file test.h
