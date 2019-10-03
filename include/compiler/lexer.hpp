#pragma once

#include <stdexcept>
#include <string>
#include "source.hpp"
#include "token.hpp"

namespace cnt_lang::compiler
{

	class lexer_exception : public std::runtime_error
	{
	public:
		lexer_exception(const source& src, const std::string& error);
		lexer_exception(const std::string& file, const std::string& error, int line, int col);

		using std::runtime_error::what;
	};

	token next_token(source& src);
	const std::string& get_token_name(token_type type);
}
