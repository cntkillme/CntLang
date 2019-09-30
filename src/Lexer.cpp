#include <array>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include "Lexer.hpp"

using namespace CntLang::Compiler;

void Assert(bool cond, std::string error, const FileInfo& file)
{
	if (!cond)
		throw LexerException(std::move(error), file);
}

void Assert(bool cond, std::string error, std::string file, int line, int column)
{
	if (!cond)
		throw LexerException(std::move(error), std::move(file), line, column);
}

bool IsWhitespace(char chr) noexcept
{
	return chr == ' ' || chr == '\t' || chr == '\n';
}

bool IsDigit(char chr) noexcept
{
	return chr >= '0' && chr <= '9';
}

bool IsIdentifierStart(char chr) noexcept
{
	return (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr == '_';
}

bool IsIdentifierPart(char chr) noexcept
{
	return IsIdentifierStart(chr) || IsDigit(chr);
}

void SkipLine(FileInfo& file)
{
	file.ignore('\n');
}

void SkipWhitespace(FileInfo& file)
{
	file.consume([](char chr) { return IsWhitespace(chr); });
}

void SkipComment(FileInfo& file)
{
	for (char chr = file.next(); !(chr == '*' && file.peek() == '/'); chr = file.next())
		Assert(chr != FileInfo::eof, "end of long comment expected", file);
	file.next();
}

static auto ReadOperator(FileInfo& file) -> TokenType
{
	static const std::unordered_map<char, TokenType> prefix({
		{ ';', TokenType::Semicolon },
		{ ',', TokenType::Delimiter },
		{ '{', TokenType::LeftBrace },
		{ '}', TokenType::RightBrace },
		{ '[', TokenType::LeftBracket },
		{ ']', TokenType::RightBracket },
		{ '(', TokenType::LeftParenthesis },
		{ ')', TokenType::RightParenthesis },
		{ '&', TokenType::And },
		{ '|', TokenType::Or },
		{ '+', TokenType::Addition },
		{ '-', TokenType::Subtraction },
		{ '*', TokenType::Multiplication },
		{ '/', TokenType::Division },
		{ '%', TokenType::Remainder },
		{ '=', TokenType::Assignment },
		{ '!', TokenType::Not },
		{ '<', TokenType::LessThan },
		{ '>', TokenType::GreaterThan }
	});

	TokenType type;

	switch (char chr = file.peek()) {
		case ';': case ',': case '{': case '}': case '[': case ']': case '(': case ')':
			type = prefix.at(chr);
			file.next();
			break;
		case '+': case '-': case '*': case '/': case '%':
		case '=': case '!': case '<': case '>': {
			TokenType pretype = prefix.at(file.next());

			if (file.peek() == '=') {
				type = static_cast<TokenType>(static_cast<std::underlying_type_t<TokenType>>(pretype) + 1);
			} else {
				if (chr == '/') {
					if (file.peek() == '/') {
						SkipLine(file);
						return TokenType::Comment;
					} else if (file.peek() == '*') {
						file.next();
						SkipComment(file);
						return TokenType::Comment;
					}
				}
			}

			type = pretype;
			break;
		}
		case '&': case '|': {
			int line = file.line();
			int column = file.column();

			type = prefix.at(file.next());

			Assert(file.peek() == chr, std::string("unknown operator ") + chr + file.peek(), file.name(), line,
				column);

			file.next();
			break;
		}
		default:
			return TokenType::EndOfFile;
	}

	return type;
}

static auto ReadInteger(FileInfo& file) -> std::string
{
	return file.consume([](char chr) { return IsDigit(chr); });
}

auto ReadNumericLiteral(FileInfo& file) -> std::tuple<std::string, TokenType>
{
	std::tuple<std::string, TokenType> pair;
	int line = file.line();
	int column = file.column();

	std::get<0>(pair) = ReadInteger(file);

	if (file.peek() == '.') {
		file.next();
		std::get<0>(pair) += '.';
		std::get<0>(pair) += ReadInteger(file);
		std::get<1>(pair) = TokenType::RealLiteral;
	} else {
		std::get<1>(pair) = TokenType::IntLiteral;
	}

	if (std::get<1>(pair) == TokenType::RealLiteral)
		Assert(std::get<0>(pair).size() >= 2, "malformed real literal", file.name(), line, column);
	else
		Assert(std::get<0>(pair).size() >= 1, "expected numeric literal", file.name(), line, column);

	return pair;
}

auto ReadKeywordOrIdentifier(FileInfo& file) -> std::tuple<std::string, TokenType>
{
	static const std::unordered_map<std::string, TokenType> keywords {
		{ "void", TokenType::Void },
		{ "bool", TokenType::Bool },
		{ "int", TokenType::Int },
		{ "real", TokenType::Real },
		{ "string", TokenType::String },
		{ "mut", TokenType::Mutable },
		{ "ref", TokenType::Reference },
		{ "if", TokenType::If },
		{ "else", TokenType::Else },
		{ "while", TokenType::While },
		{ "return", TokenType::Return },
		{ "break", TokenType::Break },
		{ "continue", TokenType::Continue },
		{ "true", TokenType::True },
		{ "false", TokenType::False }
	};

	std::tuple<std::string, TokenType> pair;
	std::string identifier = file.consume([](char chr) { return IsIdentifierPart(chr); });

	Assert(identifier.size() > 0, "expected identifier", file);

	if (auto it = keywords.find(identifier); it != keywords.end())
		std::get<1>(pair) = it->second;
	else
		std::get<1>(pair) = TokenType::Identifier;
	std::get<0>(pair) = std::move(identifier);

	return pair;
}

auto ReadStringLiteral(FileInfo& file) -> std::string
{
	throw std::runtime_error("string literals not yet implemented");
}

namespace CntLang::Compiler
{
	LexerException::LexerException(std::string error, const FileInfo& file)
	: LexerException(std::move(error), file.name(), file.line(), file.column())
	{
	}

	LexerException::LexerException(std::string error, std::string file, int line, int column)
	: m_error(std::move(error)), m_file(file), m_line(line), m_column(column)
	{
	}

	const char* LexerException::what() const noexcept
	{
		return m_error.data();
	}

	const char* LexerException::file() const noexcept
	{
		return m_file.data();
	}

	int LexerException::line() const noexcept
	{
		return m_line;
	}

	int LexerException::column() const noexcept
	{
		return m_column;
	}
}

const char* CntLang::Compiler::TokenName(TokenType type) noexcept
{
	static constexpr std::array<const char*, static_cast<std::size_t>(TokenType::StringLiteral) + 1> names({
		"<eof>",
		"<comment>",
		",",
		";",
		"{", "}",
		"[", "]",
		"(", ")",
		"&&", "||",
		"+", "+=",
		"-", "-=",
		"*", "*=",
		"/", "/=",
		"%", "%=",
		"=", "==",
		"!", "!=",
		"<", "<=",
		">", ">=",
		"void", "bool", "int", "real", "string",
		"mut", "ref",
		"if", "else", "while",
		"return", "break", "continue",
		"true", "false",
		"<identifier>", "<label>",
		"<int_literal>", "<real_literal>", "<string_literal>"
	});

	return names.at(static_cast<std::size_t>(type));
}

bool CntLang::Compiler::HasLexeme(TokenType type) noexcept
{
	using T = std::underlying_type_t<TokenType>;
	auto val = static_cast<T>(type);

	return val >= static_cast<T>(TokenType::Identifier) && val <= static_cast<T>(TokenType::StringLiteral);
}

Token CntLang::Compiler::NextToken(FileInfo& file)
{
	Token tkn;

	SkipWhitespace(file);
	tkn.line = file.line();
	tkn.column = file.column();

	if (char chr = file.peek(); chr != FileInfo::eof) {
		TokenType op = ReadOperator(file);

		if (op == TokenType::Comment) {
			return NextToken(file);
		} else if (op != TokenType::EndOfFile) {
			tkn.type = op;
		} else {
			if (IsIdentifierStart(file.peek())) {
				auto [identifier, type] = ReadKeywordOrIdentifier(file);

				if (type == TokenType::Identifier) {
					if (file.peek() == ':') {
						tkn.type = TokenType::Label;
						file.next();
					} else {
						tkn.type = TokenType::Identifier;
					}
					tkn.lexeme = std::move(identifier);
				} else {
					tkn.type = type;
				}
			} else if (IsDigit(file.peek()) or file.peek() == '.') {
				auto [value, type] = ReadNumericLiteral(file);
				tkn.type = type;
				tkn.lexeme = std::move(value);
			} else if (file.peek() == '"') {
				file.next();
				tkn.type = TokenType::StringLiteral;
				tkn.lexeme = ReadStringLiteral(file);
				file.next();
			} else {
				Assert(false, "unknown symbol", file);
			}
		}
	} else {
		tkn.type = TokenType::EndOfFile;
	}

	return tkn;
}
