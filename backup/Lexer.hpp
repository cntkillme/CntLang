#pragma once

#include <istream>
#include "Compiler/Token.hpp"

namespace CntLang::Compiler
{
	class Lexer
	{
	public:
		explicit Lexer(std::istream& source) noexcept;

		Token Next();
	private:
		std::istream& m_source;
		int m_line;
		int m_col;
		char m_char;

		void SkipWhitespace();
		char Scan();
		void SkipLine();

		static bool IsDigit(char ch) noexcept;
		static bool IsIdentifierBegin(char ch) noexcept;
		static bool IsIdentifierPart(char ch) noexcept;
		static bool IsWhitespace(char ch) noexcept;
	};

	class LexerException : std::runtime_error
	{
	public:
		LexerException(std::string message, int line, int col);

		int line() const noexcept;
		int col() const noexcept;
	private:
		int m_line;
		int m_col;
	};
}
