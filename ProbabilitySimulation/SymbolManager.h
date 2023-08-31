#pragma once

#include <string>
#include <unordered_map>
namespace Slots
{
	enum Symbol
	{
		W1,
		H1, H2, H3,
		L1, L2, L3, L4,
		B1,
		NONE
	};
}

namespace Slots
{
	class SymbolManager
	{
	public:
		static const std::string ToString(const Symbol S);
		inline const static std::unordered_map<std::string, Symbol> SymbolTable
			=  
		{
			{"W1", Symbol::W1},
			{"H1", Symbol::H1}, {"H2", Symbol::H2}, {"H3", Symbol::H3},
			{"L1", Symbol::L1}, {"L2", Symbol::L2}, {"L3", Symbol::L3}, {"L4", Symbol::L4},
			{"B1", Symbol::B1}
		};
	};
}