#pragma once

#include <functional>
#include <fstream>
#include <string>
#include <utility>

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
		std::string ignore(char chr, bool sink = false);

		template<typename Callback>
		std::string consume(Callback&& callback)
		{
			std::string consumed;

			for (; peek() != FileInfo::eof && std::invoke(std::forward<Callback>(callback), peek()); next())
				consumed += peek();

			return consumed;
		}

	private:
		std::ifstream m_source;
		std::string m_name;
		int m_tab_size;
		int m_line = 1;
		int m_column = 1;
	};
}
