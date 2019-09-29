#pragma once

#include <string>

namespace CntLang::Compiler
{
	enum class TokenType
	{
		EndOfFile,
		Semicolon,
		Delimiter,

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
		If, Then, ElseIf, Else, While, Do, End,
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
