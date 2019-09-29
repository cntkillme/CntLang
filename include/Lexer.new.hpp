#pragma once

#include <stdexcept>
#include <string>
#include "Compiler/Token.hpp"
#include "FileInfo.hpp"

namespace CntLang::Compiler
{
	class LexerException : public std::exception
	{
	public:
		LexerException(std::string error, const FileInfo& file);
		LexerException(std::string error, std::string file, int line, int column);

		const char* what() const noexcept override;
		const char* file() const noexcept;
		int line() const noexcept;
		int column() const noexcept;

	private:
		std::string m_error;
		std::string m_file;
		int m_line;
		int m_column;
	};

	Token NextToken(FileInfo& file);
}