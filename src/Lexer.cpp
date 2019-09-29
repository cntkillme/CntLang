#include "Lexer.hpp"

using namespace CntLang::Compiler;

/* Lexer Definitions */

Lexer::Lexer(std::istream& source) noexcept
: m_source(source)
, m_line(1)
, m_col(0)
, m_char(' ')
{
	// source.exceptions(source.failbit | source.badbit);
}

Token Lexer::Next()
{
	Token tokenData;

Begin:
	SkipWhitespace();
	fflush(stdout);
	tokenData.Line = m_line;
	tokenData.Col = m_col;

	switch (m_char) {
		case -1:
			tokenData.Type = TokenType::EndOfStream;
			break;
		case ';':
		case ',':
		case '{':
		case '}':
		case '[':
		case ']':
		case '(':
		case ')':
			tokenData.Type = Terminals.at(std::string{m_char});
			break;
		case '+':
		case '-':
		case '*':
		case '%':
		case '^':
			switch (char last = m_char; Scan()) {
				case '=':
					tokenData.Type = Terminals.at(std::string{last} + '=');
					break;
				default:
					tokenData.Type = Terminals.at(std::string{last});
					goto NoScan;
			}
			break;
		case '/':
			switch (Scan()) {
				case '=':
					tokenData.Type = Terminals.at("/=");
					break;
				case '/':
					SkipLine();
					goto Begin;
				case '*':
					Scan();
					while (true) {
						if (m_char == '*') {
							if (Scan() == '/') {
								Scan();
								goto Begin;
							}
						} else if (m_char == -1) {
							throw LexerException("expected end of comment", m_line, m_col);
						} else {
							Scan();
						}
					}
				default:
					tokenData.Type = Terminals.at("/");
					goto NoScan;
			}
			break;
		case '&':
		case '|':
			if (char last = m_char; last == Scan())
				tokenData.Type = Terminals.at(std::string{last} + last);
			else
				throw LexerException("unknown operator", tokenData.Line, tokenData.Col);
		case '~':
			switch (Scan()) {
				case '=':
					tokenData.Type = Terminals.at("~=");
					break;
				default:
					throw LexerException("unknown operator", tokenData.Line, tokenData.Col);
			}
			break;
		case '=':
			switch (Scan()) {
				case '=':
					tokenData.Type = Terminals.at("==");
					break;
				default:
					tokenData.Type = Terminals.at("=");
					goto NoScan;
			}
			break;
		case '<':
		case '>':
		case '!':
			switch (char last = m_char; Scan()) {
				case '=':
					tokenData.Type = Terminals.at(std::string{last} + '=');
					break;
				default:
					tokenData.Type = Terminals.at(std::string{last});
					goto NoScan;
			}
			break;
		case '"':
			tokenData.Type = TokenType::StringLiteral;
			Scan();

			while (m_char != '"') {
				switch (m_char) {
					case -1:
						throw LexerException("expected end of string marker", m_line, m_col);
					case '\\':
						switch (Scan()) {
							case '\\':
							case '\'':
							case '"':
								tokenData.Data += m_char;
								break;
							case 'n':
								tokenData.Data += '\n';
								break;
							case 't':
								tokenData.Data += '\t';
								break;
							default:
								throw LexerException("invalid escape code", m_line, m_col);
						}

						Scan();
						break;
					default:
						if (m_char >= 0x20 || m_char < 0)
							tokenData.Data += m_char;
						else
							throw LexerException("bad symbol", m_line, m_col);

						Scan();
						break;
				}
			}
			break;
		default:
			if (IsIdentifierBegin(m_char)) {
				std::string identifier;

				while (IsIdentifierPart(m_char)) {
					identifier += m_char;
					Scan();
				}

				if (auto it = Terminals.find(identifier); it != Terminals.end()) {
					tokenData.Type = it->second;
				} else {
					if (m_char == ':') {
						tokenData.Type = TokenType::Label;
						Scan();
					} else {
						tokenData.Type = TokenType::Identifier;
					}

					tokenData.Data = std::move(identifier);

				}
			} else {
				std::string number;
				bool decimal = false;

				while (IsDigit(m_char) || m_char == '.') {
					if (m_char == '.') {
						if (!decimal) {
							decimal = true;
							number += '.';
						} else {
							throw LexerException("malformed number, extra decimal point", m_line, m_col);
						}
					} else {
						number += m_char;
					}
					Scan();
				}

				if (decimal && number.size() == 1)
					throw LexerException("malformed number, expected digit", m_line, m_col);

				tokenData.Type = decimal ? TokenType::RealLiteral : TokenType::IntLiteral;
				tokenData.Data = std::move(number);
			}
			goto NoScan;
	}

	Scan();

NoScan:
	return tokenData;
}

char Lexer::Scan()
{
	char last = m_char;

	m_source.get(m_char);

	if (m_source.eof() || !m_source)
		return m_char = -1;

	if (last == '\n') {
		m_line += 1;
		m_col = 1;
	} else {
		m_col += 1;
	}

	return m_char;
}

void Lexer::SkipWhitespace()
{
	while (m_char != -1 && IsWhitespace(m_char))
		Scan();
}

void Lexer::SkipLine()
{
	while (m_char != -1 && m_char != '\n')
		Scan();
}

bool Lexer::IsWhitespace(char ch) noexcept
{
	return ch == ' ' || ch == '\n' || ch == '\t';
}

bool Lexer::IsDigit(char ch) noexcept
{
	return ch >= '0' && ch <= '9';
}

bool Lexer::IsIdentifierBegin(char ch) noexcept
{
	return ch == '_' ||
		(ch >= 'A' && ch <= 'Z') ||
		(ch >= 'a' && ch <= 'z');
}

bool Lexer::IsIdentifierPart(char ch) noexcept
{
	return IsIdentifierBegin(ch) || IsDigit(ch);
}

/* LexerException Definitions */

LexerException::LexerException(std::string message, int line, int col)
: std::runtime_error(std::move(message))
, m_line(line)
, m_col(col)
{
}

int LexerException::line() const noexcept
{
	return m_line;
}

int LexerException::col() const noexcept
{
	return m_col;
}
