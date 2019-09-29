#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Reflection/Token.hpp"

int main()
{
	std::ifstream file("test/Example1.cnt");
	CntLang::Compiler::Lexer lexer(file);
	CntLang::Compiler::Parser parser(lexer);

	while (true) {
		parser.Next();
	}

	return 0;
}
