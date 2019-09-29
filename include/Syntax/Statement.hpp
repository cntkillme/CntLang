#pragma once

namespace CntLang::Compiler::Syntax
{
	struct Statement : Node
	{
	};

	struct VariableDeclaration;
	struct Expression;

	struct VariableDefinitionStatement : Statement
	{
		std::unique_ptr<VariableDeclaration> Declaration;
		std::unique_ptr<Expression> Initializer;

		VariableDefinitionStatement(decltype(Declaration) declaration, decltype(Initializer) initializer)
		: Declaration(std::move(declaration))
		, Initializer(std::move(initializer))
		{
		}
	};

	struct FunctionDefinitionStatement : Statement
	{
		std::unique_ptr<VariableDeclaration> ReturnType;
		std::unique_ptr<Identifier> Name;
		std::vector<std::unique_ptr<VariableDeclaration>> Parameters;
		std::vector<std::unique_ptr<Statement>> Body;

		FunctionDefinitionStatement(decltype(ReturnType) returnType, decltype(Name) name) noexcept
		: ReturnType(std::move(returnType))
		, Name(std::move(name))
		{
		}
	};

	struct LabelStatement : Statement
	{
		std::unique_ptr<Identifier> Label;

		LabelStatement(decltype(Label) label) noexcept
		: Label(std::move(label))
		{
		}
	};

	struct ElseIfStatement;
	struct ElseStatement;

	struct IfStatement : Statement
	{
		std::unique_ptr<Expression> Condition;
		std::vector<std::unique_ptr<Statement>> Body;
		std::vector<std::unique_ptr<ElseIfStatement>> ElseIf;
		std::unique_ptr<ElseStatement> Else;

		IfStatement(decltype(Condition) condition) noexcept
		: Condition(std::move(condition))
		{
		}
	};

	struct ElseIfStatement : Statement
	{
		std::unique_ptr<Expression> Condition;
		std::vector<std::unique_ptr<Statement>> Body;

		ElseIfStatement(decltype(Condition) condition) noexcept
		: Condition(std::move(condition))
		{
		}
	};

	struct ElseStatement : Statement
	{
		std::vector<std::unique_ptr<Statement>> Body;
	};

	struct WhileStatement : Statement
	{
		std::unique_ptr<Expression> Condition;
		std::vector<std::unique_ptr<Statement>> Body;

		WhileStatement(decltype(Condition) condition) noexcept
		: Condition(std::move(condition))
		{
		}
	};

	struct ReturnStatement : Statement
	{
		std::unique_ptr<Expression> Result;

		ReturnStatement(decltype(Result) result) noexcept
		: Result(std::move(result))
		{
		}
	};

	struct BreakStatement : Statement
	{
		std::unique_ptr<Identifier> Label;

		BreakStatement(decltype(Label) label) noexcept
		: Label(std::move(label))
		{
		}
	};

	struct ContinueStatement : Statement
	{
		std::unique_ptr<Identifier> Label;

		ContinueStatement(decltype(Label) label) noexcept
		: Label(std::move(label))
		{
		}
	};
}
