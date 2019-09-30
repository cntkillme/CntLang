#pragma once

namespace CntLang::Compiler::Syntax
{
	struct MultiplicativeExpression;

	struct AdditiveExpression : RelationalExpression
	{
	};

	struct AdditionExpression : AdditiveExpression
	{
		std::unique_ptr<AdditiveExpression> Left;
		std::unique_ptr<MultiplicativeExpression> Right;

		AdditionExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct SubtractionExpression : AdditiveExpression
	{
		std::unique_ptr<AdditiveExpression> Left;
		std::unique_ptr<MultiplicativeExpression> Right;

		SubtractionExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};
}
