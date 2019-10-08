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

		explicit lexical_error(kind error, int line, int column);

	private:
		kind m_kind;
		int m_line;
		int m_column;
	};

	token next_token(stream_info& stream);
}
