#include <iostream>
#include <fstream>
#include "compiler/source.hpp"
#include "compiler/lexer.hpp"

int main(int argc, char** argv)
{
	using namespace cnt_lang::compiler;

	if (argc < 2) {
		std::cerr << "expected source file!\n";
		return 1;
	}

	std::ifstream file(argv[1]);

	if (!file) {
		std::cerr << "source file not found!\n";
		return 1;
	}

	source src(file, argv[1]);
	token tkn;

	do {
		tkn = next_token(src);
		std::cout << src.name() << ':' << tkn.line << ':' << tkn.col << ':';
		std::cout << " Token: ";

		switch (tkn.type) {
			case token_type::identifier:
				std::cout << "identifier" << " (" << tkn.lexeme << ")";
				break;
			case token_type::label:
				std::cout << "label" << " (" << tkn.lexeme << ")";
				break;
			case token_type::literal_int:
				std::cout << "literal_int" << " (" << tkn.lexeme << ")";
				break;
			case token_type::literal_real:
				std::cout << "literal_real" << " (" << tkn.lexeme << ")";
				break;
			case token_type::literal_string:
				std::cout << "literal_string" << " (" << tkn.lexeme << ")";
				break;
			default:
				std::cout << tkn.lexeme;
				break;
		}
		std::cout << '\n';
	} while (tkn.type != token_type::end_of_stream);


	return 0;
}
