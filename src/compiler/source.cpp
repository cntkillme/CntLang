#include "compiler/source.hpp"

namespace cnt_lang::compiler
{
	source::source(std::istream& stream, std::string name)
	: m_stream(stream)
	, m_name(std::move(name))
	{
		// skip shebang
		if (peek() == '#') {
			while (peek() != source::eof && peek() != '\n')
				get();
			get();
		}
	}

	const std::string& source::name() const noexcept
	{
		return m_name;
	}

	int source::line() const noexcept
	{
		return m_line;
	}

	int source::col() const noexcept
	{
		return m_col;
	}

	char source::peek() const noexcept
	{
		return m_stream.peek();
	}

	char source::get()
	{
		int chr = m_stream.get();

		switch (chr) {
			case eof:
				break;
			case '\r':
			case '\n':
				if (m_stream.peek() != chr && (m_stream.peek() == '\r' || m_stream.peek() == '\n'))
					m_stream.get();
				m_line += 1;
				m_col = 1;
				break;
			default:
				m_col += 1;
				break;
		}

		return chr;
	}

	char source::next()
	{
		get();
		return peek();
	}
}
