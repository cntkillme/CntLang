#pragma once

namespace CntLang::Compiler::Syntax
{
	struct Identifier;

	enum class VariableType
	{
		Void, Bool, Int, Real, String
	};

	struct VariableDeclaration : Node
	{
		VariableType Type;
		std::unique_ptr<Identifier> Name;
		bool Mutable;
		bool Reference;

		VariableDeclaration(VariableType type, std::unique_ptr<Identifier> name, bool mut = false, bool ref = false)
		: Type(type)
		, Name(std::move(name))
		, Mutable(mut)
		, Reference(ref)
		{
		}
	};
}
