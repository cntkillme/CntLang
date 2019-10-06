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

	for (cntlang::token_kind kind = cntlang::next_token(stream);
	kind != cntlang::token_kind::end_of_stream; kind = cntlang::next_token(stream)) {
		std::cout << "Read token: " << static_cast<int>(kind) << " (" << stream.data << ")\n";
	}

	return 0;
}
