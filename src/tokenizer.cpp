#include <string>
#include <unordered_map>
#include "tokenizer.hpp"

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
			return token{"<end-of-stream>", token_kind::end_of_stream, stream.line(), stream.column()};
		else if (chr == '#')
			return handle_comment(stream);
		else if (is_digit(chr))
			return handle_numeric(stream);
		else if (is_identifier_begin(chr))
			return handle_keyword(stream);
		else
			return handle_operator(stream);
	}

	token handle_comment(stream_info& stream)
	{
		std::string content;
		int line = stream.line();
		int column = stream.column();

		while (stream.peek() != '\n')
			content += stream.get();

		stream.get();

		return {std::move(content), token_kind::comment, line, column};
	}

	token handle_numeric(stream_info& stream)
	{
		std::string content;
		token_kind kind;
		int line = stream.line();
		int column = stream.column();

		content = read_integer(stream);

		if (content.size() == 0)
			throw 0; // to-do: tokenizer_error expected numeric real

		if (stream.peek() == '.') {
			kind = token_kind::literal_real;
			content += stream.get();
			content += read_integer(stream);
		} else {
			kind = token_kind::literal_int;
		}

		if (stream.peek() == 'e' || stream.peek() == 'E') {
			kind = token_kind::literal_real;
			content += stream.get();

			if (stream.peek() == '+' || stream.peek() == '-')
				content += stream.get();

			std::string exponent = read_integer(stream);
			content += exponent;

			if (exponent.size() == 0)
				throw 0; // to-do: tokenizer_error malformed real literal, no exponent
		}

		return {std::move(content), kind, line, column};
	}

	token handle_keyword(stream_info& stream)
	{
		static const std::unordered_map<std::string, token_kind> keyword_map({
			{ "true", token_kind::literal_true },
			{ "false", token_kind::literal_false },
			{ "none", token_kind::type_none },
			{ "bool", token_kind::type_bool },
			{ "int", token_kind::type_int },
			{ "real", token_kind::type_real },
			{ "mut", token_kind::modifier_mut },
			{ "let", token_kind::keyword_let },
			{ "fn", token_kind::keyword_fn },
			{ "return", token_kind::keyword_return },
			{ "end", token_kind::keyword_end },
			{ "if", token_kind::keyword_if },
			{ "elseif", token_kind::keyword_elseif },
			{ "else", token_kind::keyword_else },
			{ "then", token_kind::keyword_then },
			{ "while", token_kind::keyword_while },
			{ "for", token_kind::keyword_for },
			{ "do", token_kind::keyword_do },
			{ "break", token_kind::keyword_break },
			{ "continue", token_kind::keyword_continue },
			{ "not", token_kind::logical_not },
			{ "and", token_kind::logical_and },
			{ "or", token_kind::logical_or }
		});

		static const std::unordered_map<std::string, token_kind> intrinsic_map({
			{ "type!", token_kind::intrinsic_type },
			{ "line!", token_kind::intrinsic_line },
			{ "column!", token_kind::intrinsic_column },
			{ "dropmut!", token_kind::intrinsic_dropmut },
			{ "dropref!", token_kind::intrinsic_dropref }
		});

		int line = stream.line;
		int column = stream.column;

		while (is_identifier_part(stream.peek()))
			stream.data += stream.get();

		if (stream.data.size() == 0)
			stream.error("expected identifier", line, column);

		if (stream.peek() == '!') {
			stream.data += stream.get();

			if (auto it = intrinsic_map.find(stream.data); it != intrinsic_map.end())
				return it->second;
			else
				stream.error("unknown intrinsic", line, column);
		} else {
			if (auto it = keyword_map.find(stream.data); it != keyword_map.end())
				return it->second;
			else
				return token_kind::identifier;
		}
	}

	token handle_operator(stream_info& stream)
	{
		static const std::unordered_map<std::string, token_kind> operator_map({
			{ ",", token_kind::delimiter },
			{ ":", token_kind::colon },
			{ "(", token_kind::parenthesis_left },
			{ ")", token_kind::parenthesis_right },
			{ "&", token_kind::modifier_ref },
			{ "+", token_kind::add },
			{ "-", token_kind::subtract },
			{ "*", token_kind::multiply },
			{ "/", token_kind::divide },
			{ "%", token_kind::remainder },
			{ "=", token_kind::assign },
			{ "+=", token_kind::assign_add },
			{ "-=", token_kind::assign_subtract },
			{ "*=", token_kind::assign_multiply },
			{ "/=", token_kind::assign_divide },
			{ "%=", token_kind::assign_remainder },
			{ "==", token_kind::equal },
			{ "!=", token_kind::not_equal },
			{ "<", token_kind::less },
			{ "<=", token_kind::less_or_equal },
			{ ">", token_kind::greater },
			{ ">=", token_kind::greater_or_equal }
		});

		int line = stream.line;
		int column = stream.column;
		stream.data += std::string(1, stream.get());
		stream.data += stream.peek();

		if (auto it = operator_map.find(stream.data); it != operator_map.end()) {
			stream.get();
			return it->second;
		}

		stream.data.pop_back();

		if (auto it = operator_map.find(stream.data); it != operator_map.end()) {
			return it->second;
		}

		stream.error("unexpected symbol", line, column);
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
