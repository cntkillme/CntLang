#pragma once

namespace CntLang::Compiler::Syntax
{
	struct PrefixExpression;

	struct MultiplicativeExpression : AdditiveExpression
	{
	};

	struct MultiplicationExpression : MultiplicativeExpression
	{
		std::unique_ptr<MultiplicativeExpression> Left;
		std::unique_ptr<PrefixExpression> Right;

		MultiplicationExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct DivisionExpression : MultiplicativeExpression
	{
		std::unique_ptr<MultiplicativeExpression> Left;
		std::unique_ptr<PrefixExpression> Right;

		DivisionExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct RemainderExpression : MultiplicativeExpression
	{
		std::unique_ptr<MultiplicativeExpression> Left;
		std::unique_ptr<PrefixExpression> Right;

		RemainderExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};
}
