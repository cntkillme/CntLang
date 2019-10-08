#pragma once

#include <stdexcept>
#include "stream_info.hpp"
#include "token.hpp"

namespace cntlang
{
	class lexical_error : public std::exception
	{
	public:
		enum class kind
		{
			expected_exponent,
			unknown_intrinsic,
			unexpected_symbol
		};

		explicit lexical_error(kind error, int line, int column) noexcept;
		kind error() const noexcept;
		int line() const noexcept;
		int column() const noexcept;

	private:
		kind m_error;
		int m_line;
		int m_column;
	};

	token next_token(stream_info& stream);
}
