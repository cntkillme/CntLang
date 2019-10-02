#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include "source.hpp"
#include "token.hpp"

namespace cnt_lang::compiler
{

	class lexer_exception : public std::runtime_error
	{
	public:
		lexer_exception(const source& file, const std::string& error, std::optional<int> line = std::nullopt,
			std::optional<int> col = std::nullopt);

		using std::runtime_error::what;
	};

	token next_token(source& src);
}
