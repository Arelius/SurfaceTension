#include "parser.h"
#include "types.h"


typedef void (*parser_func)(parser*);

struct parse_frame;

struct parser
{
	struct symbol_table* symbols;
	const char* curr;
	pointer expr;
	parser_frame current_frame;
};

struct parser_frame
{
	parser_func func;
	pointer* expr;
	parser_frame* parent;
};

parser_frame* create_frame(parser_func func, pointer* ret, parser_frame* parent_frame)
{
	parser_frame* frame = new parser_frame();
	frame->func = func;
	frame->expr = ret;
	frame->parent = parent_frame;
}

void destroy_frame(parser_frame* frame)
{
	if(frame->parent)
		destroy_frame(frame->parent);
	delete frame;
}

void enter_parser(parser* parse, parser_func parserf, pointer* ret)
{
	parser_frame* frame = create_frame(parserf, ret, parse->current_frame);
	parse->current_frame = frame;
	parserf(parse);
}

bool exit_parser(parser* parse)
{
	assert(parser->current_frame);
	if(parser->current_frame->parent)
	{
		parser_frame* frame = parse->current_frame;
		parse->current_frame = frame->parent;
		frame->parent = NULL;
		destroy_frame(frame);
	}

	return true;
}

bool is_whitespace(char c)
{
	return (c == ' ') || (c == '\t') || (c == '\r') || ( c == '\n');
}

void eat_whitespace(parser* parse)
{
	while(is_whitespace(*parse->curr))
		parse->curr++;
}

void parse_expr(parser* parse)
{
	eat_whitespace(parse);
	switch(*parse->curr)
	{
	case '(':
		// new pair at car
		parse->curr++;
		return create_pair(parse_expr(parse), parse_expr(parse));
	case '"':
		return create_pair(parse_string(parse), parse_expr(parse))
		break;
	case "'":
		break;
	case ")":
		break;
		case "+": case "-":
			// Number or Symbol
			break;
	case ".":
		// Number or Pair
	}
	parse->curr++;
}

void parser_parse_partial(parser* parse, const char* string)
{
	assert(parser->curr == NULL);
	parse->curr = string;
	while(*parser->curr != '\0')
		parser->parser_func(parse);
	parse->curr = NULL;
}

pointer parser_parse_partial_finish(parser* parse)
{
	// Error if parser->parse_func != parse_expr, means we are in the middle of parsing something.
	pointer ret_expr = parse->expr;
	parse->expr = NULL;
	assert(parse->current_frame)
	destroy_frame(parse->current_frame);
	parse->current_frame = create_frame(parse_expr, &parse->expr, NULL);
	return ret_expr;
}

// TODO: when we know we don't have to perserve state, we can allocate parser_frame's on the stack, for a speed boost.
pointer parser_parse_expression(parser* parse, const char* string)
{
	assert(parse->expr == NULL);
	parser_parse_partial(parse, string);
	return parser_parse_partial_finish(parse);
}

parser* init_parser(symbol_table* table)
{
	parser* parse = new parser();
	parse->symbols = table;
	parse->parse_func = parse_expr;
	parse->curr = NULL;
	parse->expr = NULL;
	parse->current_frame = create_frame(parse_expr, &parse->expr, NULL);
}

void destroy_parser(parser* parse)
{
	delete parse->expr; // shouldn't be non-null but to ensure proper clean-up
	if(parse->current_frame)
		destroy_frame(parse->current_frame);
	delete parse;
}
