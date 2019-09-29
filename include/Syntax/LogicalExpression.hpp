#pragma once

namespace CntLang::Compiler::Syntax
{
	struct RelationalExpression;

	struct LogicalExpression : AssignmentExpression
	{
	};

	struct AndExpression : LogicalExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<RelationalExpression> Right;

		AndExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct OrExpression : LogicalExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<RelationalExpression> Right;

		OrExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};
}
