#include "FileInfo.hpp"

namespace CntLang::Compiler
{
	FileInfo::FileInfo(std::string file, int tabSize)
	: m_source(std::ifstream(file))
	, m_tab_size(tabSize)
	{
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
}