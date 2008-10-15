#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "types.h"

struct symbol_table;

const int invalid_symbol = -1;
const char initial_symbol_table_size = 20;

symbol_table* init_symbol_table();
void destroy_symbol_table(symbol_table* tbl);
symbol symbol_from_string(symbol_table* tbl, const char* string);
symbol symbol_from_string(symbol_table* tbl, const char* string, size_t len);

#endif //_SYMBOL_H_

