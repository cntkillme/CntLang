#include <iostream>
#include <fstream>
#include "tokenizer.hpp"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cerr << "expected input file!\n";
		return 1;
	}

	std::ifstream file(argv[1]);
	cntlang::stream_info stream(file, argv[1]);

	for (auto tkn = cntlang::next_token(stream);
			tkn.type != cntlang::token::kind::end_of_stream;
			tkn = cntlang::next_token(stream)) {
//		std::cout << stream.source() << ':' << tkn << '\n';
	}

	return 0;
}
