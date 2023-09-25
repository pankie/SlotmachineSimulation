#include <iostream>
#include <sstream>
#include "Reel.h"
#include "RandomGenerator.h"

Slots::Reel::Reel()
{
}

Slots::Reel::~Reel()
{
}

void Slots::Reel::Initialize(const std::vector<Symbol>& Data)
{
	ReelSymbols = Data;
}

void Slots::Reel::Spin()
{
	const int32_t Max = static_cast<int32_t>(ReelSymbols.size()) - 1;
	const int32_t Min = 0;

	ReelIndex = RandomGenerator::UniformRandom(Min, Max);
}

std::vector<Slots::Symbol> Slots::Reel::GetResult(const int32_t Rows) const
{
	std::vector<Symbol> Result(Rows);

	const size_t Size = ReelSymbols.size();
	for (int32_t i = 0; i < Rows; i++)
	{
		Result[i] = ReelSymbols[(ReelIndex + i) % Size];
	}

	return Result;
}

void Slots::Reel::Print() const
{
	std::stringstream StringStream;
	for (int32_t i = 0; i < ReelSymbols.size(); i++)
	{
		const Symbol Symbol				= ReelSymbols[i];
		const std::string SymbolString	= SymbolManager::ToString(Symbol);
		
		StringStream << i << " : " << SymbolString << '\n';
	}

	std::cout << StringStream.str() << std::endl;
}
