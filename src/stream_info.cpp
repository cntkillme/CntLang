#include <stdexcept>
#include "stream_info.hpp"

using namespace cntlang;

stream_info::stream_info(std::istream& stream, std::string source, std::size_t bufferSize)
: m_stream(stream)
, m_source(std::move(source))
{
	if (bufferSize < 4)
		throw std::invalid_argument("buffer size must be at least 4 bytes large");

	m_buffer.reserve(bufferSize);
	m_stream.rdbuf()->pubsetbuf(m_buffer.data(), bufferSize);
}

const std::string& stream_info::source() const noexcept
{
	return m_source;
}

int stream_info::line() const noexcept
{
	return m_line;
}

int stream_info::column() const noexcept
{
	return m_column;
}

char stream_info::peek()
{
	return m_stream.peek();
}

char stream_info::get()
{
	int chr = m_stream.get();

	if (chr == '\n') {
		m_line += 1;
		m_column = 1;
	} else if (chr != eof) {
		m_column += 1;
	}

	return chr;
}

char stream_info::next()
{
	get();
	return peek();
}
