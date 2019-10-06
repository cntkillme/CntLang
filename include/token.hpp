#pragma once

#include <string>
#include "token_kind.hpp"

namespace cntlang
{
	struct token
	{
		std::string lexeme;
		token_kind kind;
		int line;
		int column;
	};
}
