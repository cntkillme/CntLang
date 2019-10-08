#pragma once

#include <variant>
#include "token.hpp"

namespace cntlang
{
	struct node
	{
		using tree_type = std::vector<node>;

		enum class kind
		{
			dummy,

			program,
			variable_definition, function_definition,
			statement,
			return_stmt, if_statement, elseif_statement, else_statement,
			while_statement, for_statement, break_statement, continue_statement,
			expression, assignment_expression,
			logical_expression, relational_expression,
			additive_expression, multiplicative_expression,
			unary_expression,
			primary_expression,
			call_expression,
			intrinsic_expression
		};

		kind type;
		std::variant<std::monostate, token, tree_type> data;

		template<typename T>
		node(kind type, T&& data)
		: type(type)
		, data(std::move(data))
		{
		}

		void append(node&& child)
		{
			std::get<tree_type>(data).emplace_back(std::move(child));
		}

		void append_dummy()
		{
			std::get<tree_type>(data).emplace_back(node(kind::dummy, std::monostate()));
		}
	};
}
