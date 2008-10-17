#ifndef _TYPES_H_
#define _TYPES_H_
#include <cstring>
#include "symbol.h"

#define invalid_size -1

typedef void* pointer;
const pointer NIL = NULL;

//TODO: Dynamic build this

enum dynamic_types
{
	DT_Pair = 0,
	DT_Symbol = 1,
	DT_Int = 2,
	DT_Real = 3,
	DT_String = 4,
	DT_Char = 5,
	DT_Invalid = 6
};

struct dynamic_type
{
	dynamic_types Id;
	size_t AllocSize;
};

struct pair
{
	pointer _car;
	pointer _cdr;
};


const dynamic_type* get_type(dynamic_types typeId);

struct symbol_table;

pointer create_pair(pointer car, pointer cdr);
pointer create_symbol(symbol_table* tbl, const char* sym);
pointer create_symbol(symbol_table* tbl, const char* sym, size_t len);
pointer create_int(int i);
pointer create_real(float f);
pointer create_char(char c);
pointer create_string(const char* str);
pointer create_string(const char* str, size_t len);

void destroy_list(pointer P);

pointer pair_car(pointer P);
pointer pair_cdr(pointer P);

char* get_string(pointer P);
pointer alloc_string(size_t len);
	

#endif //_TYPES_H_
