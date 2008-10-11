#ifndef _PARSER_H_
#define _PARSER_H_

struct parser;
struct symbol_table;

parser* init_parser(symbol_table* table);
void destroy_parser(parser* parse);

#endif //_PARSER_H_
