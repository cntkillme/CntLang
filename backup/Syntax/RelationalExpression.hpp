#pragma once

namespace CntLang::Compiler::Syntax
{
	struct AdditiveExpression;

	struct RelationalExpression : LogicalExpression
	{
	};

	struct EqualsExpression : RelationalExpression
	{
		std::unique_ptr<RelationalExpression> Left;
		std::unique_ptr<AdditiveExpression> Right;

		EqualsExpression(decltype(Left) left, decltype(Right) right)
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct NotEqualsExpression : RelationalExpression
	{
		std::unique_ptr<RelationalExpression> Left;
		std::unique_ptr<AdditiveExpression> Right;

		NotEqualsExpression(decltype(Left) left, decltype(Right) right)
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct LessThanOrEqualExpression : RelationalExpression
	{
		std::unique_ptr<RelationalExpression> Left;
		std::unique_ptr<AdditiveExpression> Right;

		LessThanOrEqualExpression(decltype(Left) left, decltype(Right) right)
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};


	struct GreaterThanOrEqualExpression : RelationalExpression
	{
		std::unique_ptr<RelationalExpression> Left;
		std::unique_ptr<AdditiveExpression> Right;

		GreaterThanOrEqualExpression(decltype(Left) left, decltype(Right) right)
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct LessThanExpression : RelationalExpression
	{
		std::unique_ptr<RelationalExpression> Left;
		std::unique_ptr<AdditiveExpression> Right;

		LessThanExpression(decltype(Left) left, decltype(Right) right)
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};


	struct GreaterThanExpression : RelationalExpression
	{
		std::unique_ptr<RelationalExpression> Left;
		std::unique_ptr<AdditiveExpression> Right;

		GreaterThanExpression(decltype(Left) left, decltype(Right) right)
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};
}
