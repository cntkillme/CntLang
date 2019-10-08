#include "parser.hpp"
#include "tokenizer.hpp"

namespace cntlang
{
	parser_error::parser_error(kind error, int line, int column) noexcept
	: m_error(error)
	, m_line(line)
	, m_column(column)
	{
	}

	parser_error::kind parser_error::error() const noexcept
	{
		return m_error;
	}

	int parser_error::line() const noexcept
	{
		return m_line;
	}

	int parser_error::column() const noexcept
	{
		return m_column;
	}
}

namespace cntlang
{
	using tree_type = typename node::tree_type;

	void scan(stream_info& stream, token& current);
	void expect(stream_info& stream, token& current, typename token::kind kind, typename parser_error::kind error);

	node parse_program(stream_info& stream, token& current);
	node parse_variable_definition(stream_info& stream, token& current);
	node parse_function_definition(stream_info& stream, token& current);
	node parse_statement(stream_info& stream)

	void parse_type(stream_info& stream, token& current, node& node);
	void parse_declaration(stream_info& stream, token& current, node& node);

	node parse(stream_info& stream)
	{
		token current;

		scan(stream, current);
		return parse_program(stream, current);
	}

	void scan(stream_info& stream, token& current)
	{
		current = next_token(stream);
	}

	void expect(stream_info& stream, token& current, typename token::kind kind, typename parser_error::kind error)
	{
		scan(stream, current);

		if (current.type != kind)
			throw parser_error(error, current.line, current.column);
	}

	node parse_program(stream_info& stream, token& current)
	{
		node program(node::kind::program, tree_type());

		while (current.type != token::kind::end_of_stream) {
			if (current.type == token::kind::keyword_let)
				program.append(parse_variable_definition(stream, current));
			else if (current.type == token::kind::keyword_fn)
				program.append(parse_function_definition(stream, current));
			else
				throw parser_error(parser_error::kind::global_expected, current.line, current.column);
		}

		return program;
	}

	node parse_variable_definition(stream_info& stream, token& current)
	{
		node definition(node::kind::variable_definition, tree_type());

		scan(stream, current); // skip let
		parse_declaration(stream, current, definition);
	}
}
