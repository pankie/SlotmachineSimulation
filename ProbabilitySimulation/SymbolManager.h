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
		static std::string ToString(Symbol S);
		inline const static std::unordered_map<std::string, Symbol> SymbolTable
			=  
		{
			{"W1", W1},
			{"H1", H1}, {"H2", H2}, {"H3", H3},
			{"L1", L1}, {"L2", L2}, {"L3", L3}, {"L4", L4},
			{"B1", B1}
		};
	};
}