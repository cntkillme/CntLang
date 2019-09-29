#include "Parser.hpp"
#include "Reflection/Token.hpp"

using namespace CntLang::Compiler;

Parser::Parser(Lexer& lexer) noexcept
: m_source(lexer)
{
}

Node& Parser::Next()
{
	ParsePrefixExpression();
}

Token& Parser::Scan()
{
	return m_token = m_source.Next();
}

void Parser::Expect(TokenType type)
{
	if (m_token.Type == type)
		Scan();
	else
		ExpectedTokenError(type);
}

[[noreturn]] void Parser::ExpectedTokenError(TokenType type) const
{
	throw ParserException(std::string("expected token ") +
		Reflection::TokenNames.at(static_cast<std::size_t>(type)) +
		"at " + std::to_string(m_token.Line) + ':' + std::to_string(m_token.Col));
}

[[noreturn]] void Parser::UnexpectedTokenError(TokenType type) const
{
	throw ParserException(std::string("unexpected token ") +
		Reflection::TokenNames.at(static_cast<std::size_t>(type)) +
		"at " + std::to_string(m_token.Line) + ':' + std::to_string(m_token.Col));
}

std::vector<std::unique_ptr<Syntax::Statement>> Parser::ParseIfBody()
{
	std::vector<std::unique_ptr<Syntax::Statement>> body;

	while (true) {
		switch (m_token.Type) {
			case TokenType::ElseIf:
			case TokenType::Else:
			case TokenType::End:
				goto Finished;
			default:
				body.emplace_back(ParseStatement());
				break;
		}
	}

Finished:
	return body;
}

std::unique_ptr<Syntax::Statement> Parser::ParseStatement()
{
	std::unique_ptr<Syntax::Statement> node;

	switch (m_token.Type) {
		case TokenType::If: {
			std::unique_ptr<Syntax::IfStatement> ifStmt;
			Scan();
			ifStmt = std::make_unique<Syntax::IfStatement>(ParseExpression());
			Expect(TokenType::Then);
			ifStmt->Body = ParseIfBody();

			while (m_token.Type == TokenType::ElseIf) {
				std::unique_ptr<Syntax::ElseIfStatement> elseIfStmt;
				Scan();
				elseIfStmt = std::make_unique<Syntax::ElseIfStatement>(ParseExpression());
				Expect(TokenType::Then);
				elseIfStmt->Body = ParseIfBody();
				ifStmt->ElseIf.emplace_back(std::move(elseIfStmt));
			}

			if (m_token.Type == TokenType::Else) {
				std::unique_ptr<Syntax::ElseStatement> elseStmt;
				Scan();
				elseStmt = std::make_unique<Syntax::ElseStatement>();
				elseStmt->Body = ParseIfBody();
			}

			Expect(TokenType::End);

			node = std::move(ifStmt);
			break;
		}
		case TokenType::While:

		case TokenType::Return:

		case TokenType::Break:

		case TokenType::Continue:

		default:
			break;
	}
}

std::unique_ptr<Syntax::Expression> Parser::ParseExpression()
{
	return ParseAssignmentExpression();
}

std::unique_ptr<Syntax::AssignmentExpression> Parser::ParseAssignmentExpression()
{
	auto node = ParseLogicalExpression();

	switch (m_token.Type) {
		case TokenType::Assignment:
			Scan();
			return std::make_unique<Syntax::DirectAssignmentExpression>(std::move(node), ParseAssignmentExpression());
		case TokenType::AdditionAssignment:
			Scan();
			return std::make_unique<Syntax::AdditionAssignmentExpression>(std::move(node), ParseAssignmentExpression());
		case TokenType::SubtractionAssignment:
			Scan();
			return std::make_unique<Syntax::SubtractionAssignmentExpression>(std::move(node),
				ParseAssignmentExpression());
		case TokenType::MultiplicationAssignment:
			Scan();
			return std::make_unique<Syntax::MultiplicationAssignmentExpression>(std::move(node),
				ParseAssignmentExpression());
		case TokenType::DivisionAssignment:
			Scan();
			return std::make_unique<Syntax::DivisionAssignmentExpression>(std::move(node), ParseAssignmentExpression());
		case TokenType::RemainderAssignment:
			Scan();
			return std::make_unique<Syntax::RemainderAssignmentExpression>(std::move(node),
				ParseAssignmentExpression());
		case TokenType::ExponentiationAssignment:
			Scan();
			return std::make_unique<Syntax::ExponentiationAssignmentExpression>(std::move(node),
				ParseAssignmentExpression());
		default:
			return node;
	}
}

std::unique_ptr<Syntax::LogicalExpression> Parser::ParseLogicalExpression()
{
	std::unique_ptr<Syntax::LogicalExpression> node = ParseRelationalExpression();

	while (true) {
		switch (m_token.Type) {
			case TokenType::And:
				Scan();
				node = std::make_unique<Syntax::AndExpression>(std::move(node),
					ParseRelationalExpression());
				break;
			case TokenType::Or:
				Scan();
				node = std::make_unique<Syntax::OrExpression>(std::move(node),
					ParseRelationalExpression());
				break;
			default:
				goto NoOperator;
		}
	}

NoOperator:
	return node;
}


std::unique_ptr<Syntax::RelationalExpression> Parser::ParseRelationalExpression()
{
	std::unique_ptr<Syntax::RelationalExpression> node = ParseAdditiveExpression();

	while (true) {
		switch (m_token.Type) {
			case TokenType::Equals:
				Scan();
				node = std::make_unique<Syntax::EqualsExpression>(std::move(node), ParseAdditiveExpression());
				break;
			case TokenType::NotEquals:
				Scan();
				node = std::make_unique<Syntax::NotEqualsExpression>(std::move(node), ParseAdditiveExpression());
				break;
			case TokenType::LessThanOrEqual:
				Scan();
				node = std::make_unique<Syntax::LessThanOrEqualExpression>(std::move(node), ParseAdditiveExpression());
				break;
			case TokenType::GreaterThanOrEqual:
				Scan();
				node = std::make_unique<Syntax::GreaterThanOrEqualExpression>(std::move(node),
					ParseAdditiveExpression());
				break;
			case TokenType::LessThan:
				Scan();
				node = std::make_unique<Syntax::LessThanExpression>(std::move(node), ParseAdditiveExpression());
				break;
			case TokenType::GreaterThan:
				Scan();
				node = std::make_unique<Syntax::GreaterThanExpression>(std::move(node), ParseAdditiveExpression());
				break;
			default:
				goto NoOperator;
		}
	}

NoOperator:
	return node;
}

std::unique_ptr<Syntax::AdditiveExpression> Parser::ParseAdditiveExpression()
{
	std::unique_ptr<Syntax::AdditiveExpression> node = ParseMultiplicativeExpression();

	while (true) {
		switch (m_token.Type) {
			case TokenType::Addition:
				Scan();
				node = std::make_unique<Syntax::AdditionExpression>(std::move(node),
					ParseMultiplicativeExpression());
				break;
			case TokenType::Subtraction:
				Scan();
				node = std::make_unique<Syntax::SubtractionExpression>(std::move(node),
					ParseMultiplicativeExpression());
				break;
			default:
				goto NoOperator;
		}
	}

NoOperator:
	return node;
}

std::unique_ptr<Syntax::MultiplicativeExpression> Parser::ParseMultiplicativeExpression()
{
	std::unique_ptr<Syntax::MultiplicativeExpression> node = ParsePrefixExpression();

	while (true) {
		switch (m_token.Type) {
			case TokenType::Multiplication:
				Scan();
				node = std::make_unique<Syntax::MultiplicationExpression>(std::move(node), ParsePrefixExpression());
				break;
			case TokenType::Division:
				Scan();
				node = std::make_unique<Syntax::DivisionExpression>(std::move(node), ParsePrefixExpression());
				break;
			case TokenType::Remainder:
				Scan();
				node = std::make_unique<Syntax::RemainderExpression>(std::move(node), ParsePrefixExpression());
				break;
			default:
				goto NoOperator;
		}
	}

NoOperator:
	return node;
}

std::unique_ptr<Syntax::PrefixExpression> Parser::ParsePrefixExpression()
{
	switch (m_token.Type) {
		case TokenType::Subtraction:
			Scan();
			return std::make_unique<Syntax::NegationExpression>(std::move(ParsePostfixExpression()));
		case TokenType::Not:
			Scan();
			return std::make_unique<Syntax::NotExpression>(std::move(ParsePostfixExpression()));
		default:
			return ParsePostfixExpression();
	}
}

std::unique_ptr<Syntax::PostfixExpression> Parser::ParsePostfixExpression()
{
	auto node = ParsePrimaryExpression();

	switch (m_token.Type) {
		case TokenType::Exponentiation:
			Scan();
			return std::make_unique<Syntax::ExponentiationExpression>(std::move(node), ParsePostfixExpression());
		default:
			return node;
	}
}

std::unique_ptr<Syntax::PrimaryExpression> Parser::ParsePrimaryExpression()
{
	std::unique_ptr<Syntax::PrimaryExpression> node;

	switch (m_token.Type) {
		case TokenType::LeftParenthesis:
			Scan();
			node = std::make_unique<Syntax::GroupExpression>(ParseExpression());
			Expect(TokenType::RightParenthesis);
			goto NoScan;
		case TokenType::True:
			node = std::make_unique<Syntax::BoolLiteral>(true);
			break;
		case TokenType::False:
			node = std::make_unique<Syntax::BoolLiteral>(false);
			break;
		case TokenType::IntLiteral:
			node = std::make_unique<Syntax::IntLiteral>(m_token.Data);
			break;
		case TokenType::RealLiteral:
			node = std::make_unique<Syntax::RealLiteral>(m_token.Data);
			break;
		case TokenType::StringLiteral:
			node = std::make_unique<Syntax::StringLiteral>(std::move(m_token.Data));
			break;
		case TokenType::Identifier: {
			auto identifier = std::make_unique<Syntax::Identifier>(std::move(m_token.Data));

			switch (Scan().Type) {
				case TokenType::LeftBracket:
					Scan();
					node = std::make_unique<Syntax::IndexExpression>(std::move(identifier), ParseExpression());
					break;
				case TokenType::LeftParenthesis: {
					auto callExpr = std::make_unique<Syntax::CallExpression>(std::move(identifier));
					Scan();

					if (m_token.Type != TokenType::RightParenthesis) {
						while (true) {
							callExpr->Arguments.emplace_back(ParseExpression());

							if (m_token.Type == TokenType::RightParenthesis)
								break;
							Expect(TokenType::Delimiter);
						}
					}

					Expect(TokenType::RightParenthesis);
					node = std::move(callExpr);
					break;
				}
				default:
					node = std::move(identifier);
					break;
			}
			goto NoScan;
		}
		default:
			UnexpectedTokenError(m_token.Type);
	}

	Scan();

NoScan:
	return node;
}


ParserException::ParserException(std::string message)
: std::runtime_error(std::move(message))
{
}
