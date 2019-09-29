#pragma once

namespace CntLang::Compiler::Syntax
{
	struct LogicalExpression;

	struct AssignmentExpression : Expression
	{
	};

	struct DirectAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		DirectAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct AdditionAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		AdditionAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct SubtractionAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		SubtractionAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct MultiplicationAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		MultiplicationAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct DivisionAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		DivisionAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct RemainderAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		RemainderAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};

	struct ExponentiationAssignmentExpression : AssignmentExpression
	{
		std::unique_ptr<LogicalExpression> Left;
		std::unique_ptr<AssignmentExpression> Right;

		ExponentiationAssignmentExpression(decltype(Left) left, decltype(Right) right) noexcept
		: Left(std::move(left))
		, Right(std::move(right))
		{
		}
	};
}
