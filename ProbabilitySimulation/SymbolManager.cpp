#include "SymbolManager.h"

const std::string Slots::SymbolManager::ToString(const Slots::Symbol Symbol)
{
	auto Iterator = std::find_if(std::begin(SymbolManager::SymbolTable), std::end(SymbolManager::SymbolTable), 
		[&Symbol](auto&& Pair) 
		{
			return Pair.second == Symbol; 
		}
	);

	if (Iterator == std::end(SymbolManager::SymbolTable))
	{
		return "SYMBOL NOT FOUND";
	}

	return Iterator->first;
}
