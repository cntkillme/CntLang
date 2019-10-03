#pragma once

#include <string>
#include "token_kind.hpp"

namespace cnt_lang::compiler
{
	class Token
	{
	public:
		token() noexcept = default;
		token(const token& token);
		token(token_type type, const source& src) noexcept;
		token(token_type type, int line, int col) noexcept;

		virtual ~token() = default;

		auto name() const noexcept -> const std::string&;
		auto type() const noexcept -> token_type;
		auto line() const noexcept -> int;
		auto col() const noexcept -> int;

		operator bool() const noexcept;

		virtual auto data() const noexcept -> const std::string&;

	protected:
		token_kind m_type = token_type::end_of_stream;
		int m_line = 0;
		int m_col = 0;
	};

	class complex_token : public token
	{
	public:
		complex_token(std::string lexeme = "");

		const std::string& lexeme() const noexcept;

	private:
		std::string m_lexeme;
	};
}
