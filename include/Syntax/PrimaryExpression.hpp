#pragma once

namespace CntLang::Compiler::Syntax
{
	struct PrimaryExpression : PostfixExpression
	{
	};

	struct GroupExpression : PrimaryExpression
	{
		std::unique_ptr<Syntax::Expression> Body;

		GroupExpression(decltype(Body) body) noexcept
		: Body(std::move(body))
		{
		}
	};

	struct BoolLiteral : PrimaryExpression
	{
		bool Value;

		BoolLiteral(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}
	};

	struct IntLiteral : PrimaryExpression
	{
		long long int Value;

		IntLiteral(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}

		IntLiteral(const std::string& number)
		: Value(std::stoll(number))
		{
		}
	};

	struct RealLiteral : PrimaryExpression
	{
		double Value;

		RealLiteral(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}

		RealLiteral(const std::string& number)
		: Value(std::stod(number))
		{
		}
	};

	struct StringLiteral : PrimaryExpression
	{
		std::string Value;

		StringLiteral(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}
	};

	struct Identifier : PrimaryExpression
	{
		std::string Value;

		Identifier(decltype(Value) value) noexcept
		: Value(std::move(value))
		{
		}
	};

	struct IndexExpression : PrimaryExpression
	{
		std::unique_ptr<Identifier> Object;
		std::unique_ptr<Expression> Index;

		IndexExpression(decltype(Object) object, decltype(Index) index) noexcept
		: Object(std::move(object))
		, Index(std::move(index))
		{
		}
	};

	struct CallExpression : PrimaryExpression
	{
		std::unique_ptr<Identifier> Object;
		std::vector<std::unique_ptr<Expression>> Arguments;

		CallExpression(decltype(Object) object) noexcept
		: Object(std::move(object))
		{
		}
	};
}
