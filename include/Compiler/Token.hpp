#pragma once

#include <string>

namespace CntLang::Compiler
{
	enum class TokenType
	{
		EndOfFile,
		Comment,
		Delimiter,
		Semicolon,

		LeftBrace, RightBrace,
		LeftBracket, RightBracket,
		LeftParenthesis, RightParenthesis,

		And, Or,

		Addition, AdditionAssignment,
		Subtraction, SubtractionAssignment,
		Multiplication, MultiplicationAssignment,
		Division, DivisionAssignment,
		Remainder, RemainderAssignment,

		Assignment, Equals,
		Not, NotEquals,
		LessThan, LessThanOrEqual,
		GreaterThan, GreaterThanOrEqual,

		Void, Bool, Int, Real, String,

		Mutable, Reference,
		If, Else, While,
		Return, Break, Continue,

		True, False,

		Identifier, Label,
		IntLiteral, RealLiteral, StringLiteral
	};

	struct Token
	{
		std::string lexeme;
		TokenType type;
		int line;
		int column;
	};
}
