#include <iostream>
#include "FileInfo.hpp"
#include "Lexer.hpp"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cerr << "expected file\n";
		return 1;
	}

	try {
		CntLang::Compiler::FileInfo file(argv[1]);
		CntLang::Compiler::Token tkn;

		while ((tkn = CntLang::Compiler::NextToken(file)).type != CntLang::Compiler::TokenType::EndOfFile) {
			std::cout << file.name() << ':' << tkn.line << ':' << tkn.column << ':';
			std::cout << " Token: " << CntLang::Compiler::TokenName(tkn.type);

			if (CntLang::Compiler::HasLexeme(tkn.type))
				std::cout << " (" << tkn.lexeme << ')';
			std::cout << '\n';
		}

		std::cout << "Done!\n";
	} catch (const CntLang::Compiler::LexerException& e) {
		std::cerr << e.file() << ':' << e.line() << ':' << e.column() << ':';
		std::cerr << " lex error: " << e.what() << '\n';
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "error " << e.what() << '\n';
		return 1;
	}

	return 0;
}
