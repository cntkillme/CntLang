#pragma once

#include <string>

namespace cnt_lang::compiler
{
	enum class token_type : int;

	struct token
	{
		std::string lexeme;
		token_type type;
		int line;
		int col;
	};

	enum class token_type
	{
		end_of_stream, comment, delimiter, semicolon,
		brace_left, brace_right, bracket_left, bracket_right, parenthesis_left, parenthesis_right,

		plus, minus, times, divide, remainder,
		shift_left, shift_right, shift_arithmetic_right,
		bit_not, bit_and, bit_or, bit_xor,
		logical_not, logical_and, logical_or,

		assign,
		assign_plus, assign_minus, assign_times, assign_divide, assign_remainder,
		assign_shift_left, assign_shift_right, assign_shift_arithmetic_right,
		assign_bit_and, assign_bit_or, assign_bit_xor,

		increment, decrement,

		equals, not_equals, less_than, less_equal, greater_than, greater_equal,

		type_void, type_bool, type_int, type_real, type_string,
		modifier_mut, modifier_ref,

		intrinsic_decltype, intrinsic_dropref, intrinsic_dropmut, intrinsic_line,

		keyword_if, keyword_else, keyword_while, keyword_return, keyword_break, keyword_continue,

		literal_true, literal_false,

		identifier, label, literal_int, literal_real, literal_string
	};
}
