#pragma once

#include <unordered_map>
#include <stdexcept>
#include "stream_info.hpp"
#include "node.hpp"

namespace cntlang
{
	class parser_error : public std::exception
	{
	public:
		enum class kind
		{
			global_expected,
			/* to-do */
		};

		explicit parser_error(kind error, int line, int column) noexcept;
		kind error() const noexcept;
		int line() const noexcept;
		int column() const noexcept;

	private:
		kind m_error;
		int m_line;
		int m_column;
	};

	class parser
	{
	public:
		explicit parser(stream_info& stream);

		const node& parse();

	private:
		using declaration_list = std::unordered_map<std::string, const node* const>;

		stream_info& m_stream;
		token m_token;
		node m_program;
		declaration_list m_globals_view;
		std::unordered_map<node*, declaration_list> m_variables_view;
	};
}
