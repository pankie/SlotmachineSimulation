#include "Coin.h"
#include <sstream> 
#include <iomanip>

// Accumulated weight from coin weighted table
constexpr float TotalTableWeight = 1401.0f;

Slots::Coin::Coin(const Coin& Other)
	: Value(Other.Value)
	, Weight(Other.Weight)
	, Probability(Other.Weight / TotalTableWeight)
{
}

Slots::Coin::Coin(const int32_t InValue, const int32_t InWeight)
	: Value(InValue)
	, Weight(InWeight)
	, Probability(InWeight / TotalTableWeight)
{
}

Slots::Coin::~Coin()
{

}

std::string Slots::Coin::ToString() const
{
	std::ostringstream Table;
	Table << std::setw(15) << std::left << "Value"			<< '|' << std::setw(15) << std::right << Value << '\n';
	Table << std::setw(15) << std::left << "Weight" 		<< '|' << std::setw(15) << std::right << Weight << '\n';
	Table << std::setw(15) << std::left << "Probability"	<< '|' << std::setw(15) << std::right << Probability << '\n';
	Table << "-------------------------------";
	return Table.str();
}

