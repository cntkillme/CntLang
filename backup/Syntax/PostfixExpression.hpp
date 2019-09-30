#pragma once

namespace CntLang::Compiler::Syntax
{
	struct PrimaryExpression;

	struct PostfixExpression : PrefixExpression
	{
	};

	struct ExponentiationExpression : PostfixExpression
	{
		std::unique_ptr<PrimaryExpression> Left;
		std::unique_ptr<PostfixExpression> Right;

		ExponentiationExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};
}
