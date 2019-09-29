#pragma once

#include "Lexer.hpp"
#include "AbstractSyntaxTree.hpp"

namespace CntLang::Compiler
{
	class Parser
	{
	public:
		explicit Parser(Lexer& lexer) noexcept;
		Node& Next();

	private:
		Lexer& m_source;
		Token m_token;

		Token& Scan();
		void Expect(TokenType type);
		[[noreturn]] void ExpectedTokenError(TokenType type) const;
		[[noreturn]] void UnexpectedTokenError(TokenType type) const;

		std::vector<std::unique_ptr<Syntax::Statement>> ParseIfBody();

		std::unique_ptr<Syntax::Statement> ParseStatement();
		std::unique_ptr<Syntax::Expression> ParseExpression();
		std::unique_ptr<Syntax::AssignmentExpression> ParseAssignmentExpression();
		std::unique_ptr<Syntax::LogicalExpression> ParseLogicalExpression();
		std::unique_ptr<Syntax::RelationalExpression> ParseRelationalExpression();
		std::unique_ptr<Syntax::AdditiveExpression> ParseAdditiveExpression();
		std::unique_ptr<Syntax::MultiplicativeExpression> ParseMultiplicativeExpression();
		std::unique_ptr<Syntax::PrefixExpression> ParsePrefixExpression();
		std::unique_ptr<Syntax::PostfixExpression> ParsePostfixExpression();
		std::unique_ptr<Syntax::PrimaryExpression> ParsePrimaryExpression();
	};

	class ParserException : public std::runtime_error
	{
	public:
		ParserException(std::string message);
	};
}
