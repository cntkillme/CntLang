#pragma once

#include <string>

namespace cntlang
{
	struct token
	{
		enum class kind
		{
			end_of_stream, whitespace, comment,
			identifier,
			literal_true, literal_false,
			literal_int, literal_real,
			delimiter, colon,
			parenthesis_left, parenthesis_right,
			intrinsic_type, intrinsic_line, intrinsic_column, intrinsic_dropmut, intrinsic_dropref,
			type_none, type_bool, type_int, type_real,
			modifier_mut, modifier_ref,
			keyword_let,
			keyword_fn, keyword_return, keyword_end,
			keyword_if, keyword_elseif, keyword_else, keyword_then,
			keyword_while, keyword_for, keyword_do, keyword_break, keyword_continue,
			add, subtract, multiply, divide, remainder,
			assign, assign_add, assign_subtract, assign_multiply, assign_divide, assign_remainder,
			equal, not_equal, less, less_or_equal, greater, greater_or_equal,
			logical_not, logical_and, logical_or
		};

		std::string lexeme;
		kind type;
		int line;
		int column;
	};
}
