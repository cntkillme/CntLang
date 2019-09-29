#pragma once

namespace CntLang::Compiler::Syntax
{
	struct PostfixExpression;

	struct PrefixExpression : MultiplicativeExpression
	{
	};

	struct NegationExpression : PrefixExpression
	{
		std::unique_ptr<PostfixExpression> Value;

		NegationExpression(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}
	};

	struct NotExpression : PrefixExpression
	{
		std::unique_ptr<PostfixExpression> Value;

		NotExpression(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}
	};
}
