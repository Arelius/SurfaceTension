#include <stdio.h>
#include "symbol.h"

int main(int argc, char** argv)
{
	symbol_table* tbl = init_symbol_table();

	printf("Hello:%d\n", symbol_from_string(tbl, "Hello").Id);
	printf("hello:%d\n", symbol_from_string(tbl, "hello").Id);
	printf("heyme:%d\n", symbol_from_string(tbl, "heyme").Id);
	printf("_why:%d\n", symbol_from_string(tbl, "_why").Id);
	printf("_why:%d\n", symbol_from_string(tbl, "_why").Id);
	printf("_Why:%d\n", symbol_from_string(tbl, "_Why").Id);
	
	destroy_symbol_table(tbl);
	return 0;
}
