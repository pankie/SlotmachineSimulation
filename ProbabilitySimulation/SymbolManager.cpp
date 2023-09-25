#include "SymbolManager.h"

std::string Slots::SymbolManager::ToString(const Symbol Symbol)
{
	const auto Iterator = std::find_if(std::begin(SymbolTable), std::end(SymbolTable), 
		[&Symbol](auto&& Pair) 
		{
			return Pair.second == Symbol; 
		}
	);

	if (Iterator == std::end(SymbolTable))
	{
		return "SYMBOL NOT FOUND";
	}

	return Iterator->first;
}
