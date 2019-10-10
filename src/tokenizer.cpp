#include <string>
#include <unordered_map>
#include "tokenizer.hpp"

namespace cntlang
{
	lexical_error::lexical_error(kind error, int line, int column) noexcept
	: m_error(error)
	, m_line(line)
	, m_column(column)
	{
	}


	lexical_error::kind lexical_error::error() const noexcept
	{
		return m_error;
	}

	int lexical_error::line() const noexcept
	{
		return m_line;
	}

	int lexical_error::column() const noexcept
	{
		return m_column;
	}
}

namespace cntlang
{
	token handle_comment(stream_info& stream);
	token handle_numeric(stream_info& stream);
	token handle_keyword(stream_info& stream);
	token handle_operator(stream_info& stream);

	char skip_whitespace(stream_info& stream);
	std::string read_integer(stream_info& stream);

	bool is_epsilon(char chr) noexcept;
	bool is_whitespace(char chr) noexcept;
	bool is_character(char chr) noexcept;
	bool is_newline(char chr) noexcept;
	bool is_digit(char chr) noexcept;
	bool is_identifier_begin(char chr) noexcept;
	bool is_identifier_part(char chr) noexcept;

	token next_token(stream_info& stream)
	{
		char chr = skip_whitespace(stream);

		if (is_epsilon(chr))
			return token{"<end-of-stream>", token::kind::end_of_stream, stream.line(), stream.column()};
		else if (chr == '#')
			return handle_comment(stream);
		else if (is_digit(chr))
			return handle_numeric(stream);
		else if (is_identifier_begin(chr))
			return handle_keyword(stream);
		else
			return handle_operator(stream);
	}

	token handle_comment(stream_info& stream) // peek = '#'
	{
		std::string content;
		int line = stream.line();
		int column = stream.column();

		while (stream.peek() != '\n')
			content += stream.get();

		stream.get();

		return { std::move(content), token::kind::comment, line, column };
	}

	token handle_numeric(stream_info& stream) // is_digit(peek)
	{
		std::string content;
		typename token::kind tokenKind;
		int line = stream.line();
		int column = stream.column();

		content = read_integer(stream);

		if (stream.peek() == '.') {
			tokenKind = token::kind::literal_real;
			content += stream.get();
			content += read_integer(stream);
		} else {
			tokenKind = token::kind::literal_int;
		}

		if (stream.peek() == 'e' || stream.peek() == 'E') {
			tokenKind = token::kind::literal_real;
			content += stream.get();

			if (stream.peek() == '+' || stream.peek() == '-')
				content += stream.get();

			if (std::string exponent = read_integer(stream); exponent.size() > 0)
				content += exponent;
			else
				throw lexical_error(lexical_error::kind::expected_exponent, stream.line(), stream.column());
		}

		return { std::move(content), tokenKind, line, column };
	}

	token handle_keyword(stream_info& stream) // is_identifier_part(peek)
	{
		static const std::unordered_map<std::string, typename token::kind> keyword_map({
			{ "true", token::kind::literal_true },
			{ "false", token::kind::literal_false },
			{ "none", token::kind::type_none },
			{ "bool", token::kind::type_bool },
			{ "int", token::kind::type_int },
			{ "real", token::kind::type_real },
			{ "mut", token::kind::modifier_mut },
			{ "let", token::kind::keyword_let },
			{ "fn", token::kind::keyword_fn },
			{ "return", token::kind::keyword_return },
			{ "end", token::kind::keyword_end },
			{ "if", token::kind::keyword_if },
			{ "elseif", token::kind::keyword_elseif },
			{ "else", token::kind::keyword_else },
			{ "then", token::kind::keyword_then },
			{ "while", token::kind::keyword_while },
			{ "for", token::kind::keyword_for },
			{ "do", token::kind::keyword_do },
			{ "break", token::kind::keyword_break },
			{ "continue", token::kind::keyword_continue },
			{ "not", token::kind::logical_not },
			{ "and", token::kind::logical_and },
			{ "or", token::kind::logical_or },
			{ "type!", token::kind::intrinsic_type },
			{ "line!", token::kind::intrinsic_line },
			{ "column!", token::kind::intrinsic_column },
			{ "dropmut!", token::kind::intrinsic_dropmut },
			{ "dropref!", token::kind::intrinsic_dropref }
		});

		std::string content;
		typename token::kind tokenKind;
		int line = stream.line();
		int column = stream.column();

		while (is_identifier_part(stream.peek()))
			content += stream.get();

		if (stream.peek() == '!') // intrinsic
			content += stream.get();

		if (auto it = keyword_map.find(content); it != keyword_map.end())
			tokenKind = it->second;
		else if (content.back() != '!')
			tokenKind = token::kind::identifier;
		else
			throw lexical_error(lexical_error::kind::unknown_intrinsic, line, column);

		return { std::move(content), tokenKind, line, column };
	}

	token handle_operator(stream_info& stream)
	{
		static const std::unordered_map<std::string, typename token::kind> operator_map({
			{ ",", token::kind::delimiter },
			{ ":", token::kind::colon },
			{ ";", token::kind::semicolon },
			{ "(", token::kind::parenthesis_left },
			{ ")", token::kind::parenthesis_right },
			{ "&", token::kind::modifier_ref },
			{ "+", token::kind::add },
			{ "-", token::kind::subtract },
			{ "*", token::kind::multiply },
			{ "/", token::kind::divide },
			{ "%", token::kind::remainder },
			{ "=", token::kind::assign },
			{ "+=", token::kind::assign_add },
			{ "-=", token::kind::assign_subtract },
			{ "*=", token::kind::assign_multiply },
			{ "/=", token::kind::assign_divide },
			{ "%=", token::kind::assign_remainder },
			{ "==", token::kind::equal },
			{ "!=", token::kind::not_equal },
			{ "<", token::kind::less },
			{ "<=", token::kind::less_or_equal },
			{ ">", token::kind::greater },
			{ ">=", token::kind::greater_or_equal }
		});

		std::string content;
		typename token::kind tokenKind;
		int line = stream.line();
		int column = stream.column();

		content += stream.get();
		content += stream.peek();

		if (auto it = operator_map.find(content); it != operator_map.end()) {
			stream.get();
			tokenKind = it->second;
		} else {
			content.pop_back();

			if (auto it = operator_map.find(content); it != operator_map.end())
				tokenKind = it->second;
			else
				throw lexical_error(lexical_error::kind::unexpected_symbol, line, column);
		}

		return { std::move(content), tokenKind, line, column };
	}

	char skip_whitespace(stream_info& stream)
	{
		while (is_whitespace(stream.peek()))
			stream.get();

		return stream.peek();
	}

	std::string read_integer(stream_info& stream)
	{
		std::string content;

		while (is_digit(stream.peek()))
			content += stream.get();

		return content;
	}

	bool is_epsilon(char chr) noexcept
	{
		return chr == stream_info::eof;
	}

	bool is_whitespace(char chr) noexcept
	{
		return chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n';
	}

	bool is_character(char chr) noexcept
	{
		return chr >= 0x20 && chr <= 0x7E;
	}

	bool is_newline(char chr) noexcept
	{
		return chr == '\r' || chr == '\n';
	}

	bool is_digit(char chr) noexcept
	{
		return chr >= '0' && chr <= '9';
	}

	bool is_identifier_begin(char chr) noexcept
	{
		return (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr == '_';
	}

	bool is_identifier_part(char chr) noexcept
	{
		return is_identifier_begin(chr) || is_digit(chr);
	}
}
