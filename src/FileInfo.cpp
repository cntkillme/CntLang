#include "FileInfo.hpp"

namespace CntLang::Compiler
{
	FileInfo::FileInfo(std::string file, int tabSize)
	: m_source(std::ifstream(file))
	, m_name(std::move(file))
	, m_tab_size(tabSize)
	{
		if (!m_source)
			throw std::ios_base::failure("could not open file");

		if (peek() == '#')
			ignore('\n', true);

		// settings
		while (peek() == '%') {
			std::string setting, value;
			next();

			setting = ignore(' ', true);
			value = ignore('\n', true);

			if (setting == "source-name") {
				m_name = std::move(value);
			} else if (setting == "tab-size") {
				try {
					int size = std::stoi(value);

					if (size < 0 || size > 16)
						throw std::out_of_range("setting tab-size must be between 0 and 16");
					else
						m_tab_size = size;
				} catch (const std::exception& e) {
					throw std::out_of_range("$tab-size must be between 0 and 16");
				}
			} else {
				throw std::invalid_argument("unknown setting " + setting);
			}
		}
	}

	const std::string& FileInfo::name() const noexcept
	{
		return m_name;
	}

	int FileInfo::line() const noexcept
	{
		return m_line;
	}

	int FileInfo::column() const noexcept
	{
		return m_column;
	}

	char FileInfo::peek()
	{
		return m_source.peek();
	}

	char FileInfo::next()
	{
		int chr = m_source.get();

		switch (chr) {
			case eof:
				break;
			case '\t':
				m_column += m_tab_size;
				break;
			case '\n':
				m_column = 1;
				m_line += 1;
				break;
			default:
				m_column += 1;
				break;
		}

		return static_cast<char>(chr);
	}

	std::string FileInfo::ignore(char chr, bool sink)
	{
		std::string consumsed = consume([chr](char read) { return read != chr; });

		if (sink)
			consume([chr](char read) { return read == chr; });

		return consumsed;
	}
}
