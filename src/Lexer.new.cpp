#include <tuple>
#include <type_traits>
#include <unordered_map>
#include "Lexer.new.hpp"

// LexerException
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

// Lexer Interface
namespace CntLang::Compiler::Lexer
{
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
		return (chr >= 'A' && chr <= 'Z')
		    || (chr >= 'a' && chr <= 'z')
			|| chr == '_';
	}

	bool IsIdentifierPart(char chr) noexcept
	{
		return IsIdentifierStart(chr) || IsDigit(chr);
	}

	void SkipLine(FileInfo& file)
	{
		char chr = file.peek();

		while (chr != FileInfo::eof && chr != '\n')
			chr = file.next();
		file.next();
	}

	void SkipWhitespace(FileInfo& file)
	{
		char chr = file.peek();

		while (IsWhitespace(chr))
			chr = file.next();
	}

	void SkipComment(FileInfo& file)
	{
		char chr = file.next();

		while (!(chr == '*' && file.peek() == '/')) {
			Assert(chr != FileInfo::eof, "end of long comment expected", file);
			chr = file.next();
		}
		file.next();
	}

	static auto ReadOperator(FileInfo& file) -> TokenType
	{
		static const std::unordered_map<char, TokenType> prefix {
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
		};

		TokenType type;
		
		switch (char chr = file.peek()) {
			case ';': case ',': case '{': case '}': case '[': case ']': case '(': case ')':
				type = prefix.at(chr);
				break;
			case '+': case '-': case '*': case '/': case '%':
			case '=': case '!': case '<': case '>': {
				TokenType pretype = prefix.at(file.next());
				
				if (file.peek() == '=')
					type = static_cast<TokenType>(static_cast<std::underlying_type_t<TokenType>>(pretype) + 1);
				else if (file.peek() == '/') 
					type = pretype;

				break;
			}
			case '&': case '|': {
				int line = file.line();
				int column = file.column();
				type = prefix.at(file.next());

				Assert(file.peek() == chr, std::string("unknown operator ") + chr + file.peek(), file.name(), line, column);
				break;
			}
			default:
				type = TokenType::EndOfFile;
				break;
		}
	}

	static auto ReadInteger(FileInfo& file) -> std::string
	{
		std::string integer;
		char chr = file.peek();

		while (IsDigit(chr)) {
			integer += chr;
			chr = file.next();
		}

		return integer;
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

		Assert(std::get<0>(pair).size() >= 2, "malformed numeric literal", file.name(), line, column);
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
			{ "then", TokenType::Then },
			{ "elseif", TokenType::ElseIf },
			{ "else", TokenType::Else },
			{ "while", TokenType::While },
			{ "do", TokenType::Do },
			{ "end", TokenType::End },
			{ "return", TokenType::Return },
			{ "break", TokenType::Break },
			{ "continue", TokenType::Continue },
			{ "true", TokenType::True },
			{ "false", TokenType::False }
		};

		std::tuple<std::string, TokenType> pair;
		std::string identifier;

		for (char chr = file.next(); IsIdentifierPart(chr); chr = file.next())
			identifier += chr;
		
		Assert(identifier.size() > 0, "expected identifier", file);

		if (auto it = keywords.find(identifier); it != keywords.end())
			std::get<1>(pair) = it->second;
		std::get<0>(pair) = std::move(identifier);

		return pair;
	}

	auto ReadStringLiteral(FileInfo& file) -> std::string
	{
		std::string content;

		for (char chr = file.next(); chr != '"'; chr = file.next()) {
			Assert(chr != FileInfo::eof, "end of string expected", file);
			content += chr;
		}

		file.next();

		return content;
	}
	
	Token NextToken(FileInfo& file)
	{
		char chr;
		Token tkn;

		SkipWhitespace(file);
		chr = file.peek();
		tkn.line = file.line();
		tkn.column = file.column();

		switch (chr) {
			case FileInfo::eof:
				tkn.lexeme = "<eof>";
				tkn.type = TokenType::EndOfFile;
				break;
			case ';':
			case ',':
			case '{'
		}
	}
}
