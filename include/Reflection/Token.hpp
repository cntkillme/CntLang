#pragma once

#include <array>
#include "Token.hpp"

namespace CntLang::Compiler::Reflection
{
	// Must match exactly with CntLang::Compiler::Token
	constexpr std::array<const char*, TokenCount> TokenNames {
		"<eof>",
		"Semicolon",
		"Delimiter",

		"LeftBrace", "RightBrace",
		"LeftBracket", "RightBracket",
		"LeftParenthesis", "RightParenthesis",

		"Assignment",
		"AdditionAssignment", "SubtractionAssignment",
		"MultiplicationAssignment", "DivisionAssignment",
		"RemainderAssignment",
		"ExponentiationAssignment",

		"Addition", "Subtraction",
		"Multiplication", "Division",
		"Remainder",
		"Exponentiation",

		"And", "Or", "Not",

		"Equals", "NotEquals",
		"LessThan", "LessThanOrEqual",
		"GreaterThan", "GreaterThanOrEqual",

		"Void", "Bool", "Int", "Real", "String",

		"Mutable", "Reference",
		"If", "Then", "ElseIf", "Else", "While", "Do", "End",
		"Return", "Break", "Continue",

		"True", "False",

		"Identifier", "Label",
		"IntLiteral", "RealLiteral", "StringLiteral"
	};
}
