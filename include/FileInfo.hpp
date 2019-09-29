#pragma once

#include <fstream>
#include <string>

namespace CntLang::Compiler
{
	class FileInfo
	{
	public:
		static constexpr auto eof = std::ifstream::traits_type::eof();

	public:
		explicit FileInfo(std::string file, int tabSize = 4);

		const std::string& name() const noexcept;
		int line() const noexcept;
		int column() const noexcept;
		char peek();
		char next();

	private:
		std::ifstream m_source;
		std::string m_name;
		int m_tab_size = 4;
		int m_line = 1;
		int m_column = 1;
	};
}
