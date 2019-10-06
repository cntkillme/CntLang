#include <array>
#include <unordered_map>
#include "monostate_function.hpp"
#include "compiler/lexer.hpp"

namespace cnt_lang::compiler
{
	lexer_exception::lexer_exception(const source& src, const std::string& error)
	: lexer_exception(src.name(), error, src.line(), src.col())
	{
	}

	lexer_exception::lexer_exception(const std::string& file, const std::string& error, int line, int col)
	: std::runtime_error(file + ':' + std::to_string(line) + ':' + std::to_string(col) + ": " + error)
	{
	}

	template<typename Functor>
	std::string consume(source& src, Functor&& callback)
	{
		std::string content;

		while (src.peek() != source::eof)
			if (std::forward<Functor>(callback)(src.peek()))
				content += src.get();
			else
				break;

		return content;
	}

	bool is_whitespace(char chr) noexcept
	{
		return chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n';
	}

	bool is_digit(char chr) noexcept
	{
		return chr >= '0' && chr <= '9';
	}

	bool is_identifier_begin(char chr) noexcept
	{
		return chr == '_' || (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z');
	}

	bool is_identifier_part(char chr) noexcept
	{
		return is_identifier_begin(chr) || is_digit(chr);
	}

	void skip_line(source& src)
	{
		consume(src, [](char chr) { return chr != '\n'; });
		src.get();
	}

	void skip_whitespace(source& src)
	{
		consume(src, monostate_function<is_whitespace>());
	}

	void skip_short_comment(source& src)
	{
		skip_line(src);
	}

	void skip_long_comment(source& src) // src after `/*`
	{
		while (src.peek() != source::eof) {
			if (src.get() == '*')
				if (src.get() == '/')
					return;
		}

		throw lexer_exception(src, "expected end-of-comment marker");
	}

	std::string read_integer(source& src)
	{
		return consume(src, monostate_function<is_digit>());
	}

	token try_numeric_literal(source& src)
	{
		token tkn{ "", token_type::literal_int, src.line(), src.col() };

		tkn.lexeme = read_integer(src);

		if (src.peek() == '.') {
			src.get();
			tkn.type = token_type::literal_real;
			tkn.lexeme += '.' + read_integer(src);
		}

		if (tkn.type == token_type::literal_int && tkn.lexeme.size() == 0)
			tkn.type = token_type::end_of_stream;
		else if (tkn.type == token_type::literal_real && tkn.lexeme.size() < 2)
			throw lexer_exception(src.name(), "malformed real literal", tkn.line, tkn.col);

		return tkn;
	}

	token try_string_literal(source& src)
	{
		token tkn{ "", token_type::literal_string, src.line(), src.col() };

		if (src.peek() == '"') {
			src.get();

			while (src.peek() != source::eof && src.peek() != '\r' && src.peek() != '\n') {
				switch (char chr = src.get()) {
					case '"':
						src.get();
						return tkn;
					case '\\':
						switch (char esc = src.get()) {
							case '"':
							case '\'':
							case '\\':
								tkn.lexeme += esc;
								break;
							case 'r':
								tkn.lexeme += '\r';
								break;
							case 'n':
								tkn.lexeme += '\n';
								break;
							case 't':
								tkn.lexeme += '\t';
								break;
							default:
								throw lexer_exception(src, "invalid escape character");
						}
					default:
						tkn.lexeme += chr;
						break;
				}
			}

			throw lexer_exception(src, "expected end-of-string marker");
		} else {
			tkn.type = token_type::end_of_stream;
			return tkn;
		}
	}

	token try_intrinsic(source& src)
	{
		static const std::unordered_map<std::string, token_type> intrinsic_map({
			{ "decltype", token_type::intrinsic_decltype },
			{ "dropmut", token_type::intrinsic_dropmut },
			{ "dropref", token_type::intrinsic_dropref },
			{ "line", token_type::intrinsic_line }
		});

		if (src.peek() == '@') {
			int line = src.line();
			int col = src.col();

			src.get();

			if (auto it = intrinsic_map.find(consume(src, monostate_function<is_identifier_part>()));
				it != intrinsic_map.end()) {
				return { "", it->second, line, col };
			} else {
				throw lexer_exception(src.name(), "unknown intrinsic", line, col + 1);
			}
		} else {
			return { };
		}
	}

	token try_keyword_identifier_or_label(source& src)
	{
		static const std::unordered_map<std::string, token_type> keyword_map({
			{ "void", token_type::type_void },
			{ "bool", token_type::type_bool },
			{ "int", token_type::type_int },
			{ "real", token_type::type_real },
			{ "string", token_type::type_string },
			{ "mut", token_type::modifier_mut },
			{ "ref", token_type::modifier_ref },
			{ "if", token_type::keyword_if },
			{ "if", token_type::keyword_if },
			{ "else", token_type::keyword_else },
			{ "while", token_type::keyword_while },
			{ "return", token_type::keyword_return },
			{ "break", token_type::keyword_break },
			{ "continue", token_type::keyword_continue },
			{ "true", token_type::literal_true },
			{ "false", token_type::literal_false }
		});

		token tkn{ "", token_type::identifier, src.line(), src.col() };
		tkn.lexeme = consume(src, monostate_function<is_identifier_part>());

		if (tkn.lexeme.size() == 0) {
			tkn.type = token_type::end_of_stream;
			return tkn;
		}

		if (auto it = keyword_map.find(tkn.lexeme); it != keyword_map.end()) { // keyword
			tkn.type = it->second;
		} else if (src.peek() == ':') { // label
			tkn.type = token_type::label;
			src.get();
		}

		return tkn;
	}

	token try_operator(source& src)
	{
		static const std::unordered_map<std::string, token_type> operator_map({
			{ "/*", token_type::comment },
			{ "//", token_type::comment },
			{ ",", token_type::delimiter },
			{ ";", token_type::semicolon },
			{ "{", token_type::brace_left },
			{ "}", token_type::brace_right },
			{ "[", token_type::bracket_left },
			{ "]", token_type::bracket_right },
			{ "(", token_type::parenthesis_left },
			{ ")", token_type::parenthesis_right },
			{ "+", token_type::plus },
			{ "-", token_type::minus },
			{ "*", token_type::times },
			{ "/", token_type::divide },
			{ "%", token_type::remainder },
			{ "<<", token_type::shift_left },
			{ ">>", token_type::shift_right },
			{ ">>>", token_type::shift_arithmetic_right },
			{ "~", token_type::bit_not },
			{ "&", token_type::bit_and },
			{ "|", token_type::bit_or },
			{ "^", token_type::bit_xor },
			{ "!", token_type::logical_not },
			{ "&&", token_type::logical_and },
			{ "||", token_type::logical_or },
			{ "=", token_type::assign },
			{ "+=", token_type::assign_plus },
			{ "-=", token_type::assign_minus },
			{ "*=", token_type::assign_times },
			{ "/=", token_type::assign_divide },
			{ "%=", token_type::assign_remainder },
			{ "<<=", token_type::assign_shift_left },
			{ ">>=", token_type::assign_shift_right },
			{ ">>>=", token_type::assign_shift_arithmetic_right },
			{ "&=", token_type::assign_bit_and },
			{ "|=", token_type::assign_bit_or },
			{ "^=", token_type::assign_bit_xor },
			{ "++", token_type::increment },
			{ "--", token_type::decrement },
			{ "==", token_type::equals },
			{ "!=", token_type::not_equals },
			{ "<", token_type::less_than },
			{ "<=", token_type::less_equal },
			{ ">", token_type::greater_than },
			{ ">=", token_type::greater_equal }
		});

		int line = src.line();
		int col = src.col();

		std::string read(1, src.peek());
		auto iter = operator_map.find(read);
		auto last = iter;

		while (iter != operator_map.end()) {
			if (char c = src.next(); c != source::eof) {
				read += c;
				last = iter;
				iter = operator_map.find(read);
			} else {
				break;
			}
		}

		if (last != operator_map.end()) {
			if (last->second == token_type::comment) {
				if (last->first == "//")
					skip_short_comment(src);
				else if (last->first == "/*")
					skip_long_comment(src);

				return { "<comment>", token_type::comment, line, col };
			} else {
				return { "", last->second, line, col };
			}
		} else {
			return { };
		}
	}

	token next_token(source& src)
	{
		skip_whitespace(src);

		if (char ch = src.peek(); ch != source::eof) {
			if (token tkn = try_numeric_literal(src); tkn.type != token_type::end_of_stream)
				return tkn;
			else if (token tkn = try_string_literal(src); tkn.type != token_type::end_of_stream)
				return tkn;
			else if (token tkn = try_keyword_identifier_or_label(src); tkn.type != token_type::end_of_stream)
				return tkn;
			else if (token tkn = try_operator(src); tkn.type != token_type::end_of_stream)
				if (tkn.type == token_type::comment)
					return next_token(src);
				else
					return tkn;
			else if (token tkn = try_intrinsic(src); tkn.type != token_type::end_of_stream)
				return tkn;
			else
				throw lexer_exception(src, "unexpected symbol");
		} else {
			return { "", token_type::end_of_stream, src.line(), src.col() };
		}
	}

	const std::string& get_token_name(token_type type)
	{
		static const std::array<std::string, static_cast<int>(token_type::literal_string) + 1> token_names({
			"<eos>", "<comment>", ",", ";",
			"{", "}", "[", "]", "(", ")",
			"+", "-", "*", "/", "%",
			"<<", ">>", ">>>",
			"~", "&", "|", "^",
			"!", "&&", "||",
			"=",
			"+=", "-=", "*=", "/=", "%=",
			"<<=", ">>=", ">>>=",
			"&=", "|=", "^=",
			"++", "--",
			"==", "!=", "<", "<=", ">", ">=",
			"void", "bool", "int", "real", "string",
			"mut", "ref",
			"@decltype", "@dropref", "@dropmut", "@line",
			"if", "else", "return", "break", "continue",
			"true", "false",
			"<identifier>", "<label>", "<int literal>", "<real literal>", "<string literal>"
		});

		return token_names.at(static_cast<int>(type));
	}
}
