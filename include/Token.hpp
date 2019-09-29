#pragma once

#include <type_traits>
#include <unordered_map>

namespace CntLang::Compiler
{
	enum class TokenType
	{
		EndOfStream,
		Semicolon,
		Delimiter,

		LeftBrace, RightBrace,
		LeftBracket, RightBracket,
		LeftParenthesis, RightParenthesis,

		Assignment,
		AdditionAssignment, SubtractionAssignment,
		MultiplicationAssignment, DivisionAssignment,
		RemainderAssignment,
		ExponentiationAssignment,

		Addition, Subtraction,
		Multiplication, Division,
		Remainder,
		Exponentiation,

		And, Or, Not,

		Equals, NotEquals,
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
		TokenType Type;
		int Line;
		int Col;
		std::string Data;

		constexpr operator bool() const noexcept
		{
			return Type != TokenType::EndOfStream;
		}
	};

	constexpr auto TokenCount = static_cast<std::underlying_type_t<TokenType>>(TokenType::StringLiteral) + 1;
	extern const std::unordered_map<std::string, TokenType> Terminals;
}
