#pragma once

#include <vector>
#include <cstddef>
#include <istream>
#include <string>

namespace cntlang
{
	class stream_info
	{
	public:
		static constexpr int eof = std::istream::traits_type::eof();

		stream_info(std::istream& stream, std::string source = "<unnamed>", std::size_t bufferSize = 1024);

		const std::string& source() const noexcept;
		int line() const noexcept;
		int column() const noexcept;

		char peek();
		char get();
		char next();

	private:
		std::istream& m_stream;
		std::string m_source;
		std::vector<char> m_buffer;
		int m_line = 1;
		int m_column = 1;
	};
}
