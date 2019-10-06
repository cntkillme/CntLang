#pragma once

#include <istream>
#include <string>

namespace cnt_lang::compiler
{
	class source
	{
	public:
		static constexpr auto eof = std::istream::traits_type::eof();

		explicit source(std::istream& stream, std::string name);

		const std::string& name() const noexcept;
		int line() const noexcept;
		int col() const noexcept;
		char peek() const noexcept;
		char get();
		char next();

	private:
		std::istream& m_stream;
		std::string m_name;
		int m_line = 1;
		int m_col = 1;
	};
}
