#ifndef _TYPES_H_
#define _TYPES_H_
#include <cstring>

#define invalid_size -1

typedef void* pointer;

//TODO: Dynamic build this

enum dynamic_types
{
	DT_Pair = 0,
	DT_Symbol = 1,
	DT_Int = 2,
	DT_Real = 3,
	DT_String = 4,
	DT_Invalid = 5
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

struct symbol
{
	int Id;
	symbol(int i) : Id(i)
	{}
};

const dynamic_type* get_type(dynamic_types typeId);

pointer create_pair(pointer car, pointer cdr);
pointer create_symbol(struct symbol_table* tbl, const char* sym);
pointer create_int(int i);
pointer create_real(float f);
pointer create_string(const char* str);
pointer create_string(const char* str, size_t len);
	

#endif //_TYPES_H_
