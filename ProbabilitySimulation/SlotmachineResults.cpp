#include "SlotmachineResults.h"
#include <sstream> 
#include <iomanip>
#include <iostream>

Slots::SlotmachineResults::SlotmachineResults()
{
}

Slots::SlotmachineResults::~SlotmachineResults()
{
}

void Slots::SlotmachineResults::Display() const
{
	const int32_t GamesLost = GamesPlayed - GamesWon;
	const float TotalCreditsWon = BaseGameCreditsWon + BonusCreditsWon;
	const float ReturnToPlayer = TotalCreditsWon / TotalCreditSpent;
	
	float BonusGameHitFrequency = 0.0f;
	if (FrequencyTable.find(B1) != FrequencyTable.end())
	{
		BonusGameHitFrequency = static_cast<float>(GamesPlayed) / FrequencyTable.at(B1);
	}

	std::ostringstream Table;
	Table << "\n* --------- SLOT MACHINE RESULTS --------- *" << '\n';
	Table << "\n General results: \n\n";
	Table << std::setw(22) << std::left << " Games Played" << '|' << std::setw(18) << std::right << GamesPlayed << '\n';
	Table << std::setw(22) << std::left << " Games Won" << '|' << std::setw(18) << std::right << GamesWon << '\n';
	Table << std::setw(22) << std::left << " Games Lost" << '|' << std::setw(18) << std::right << GamesLost << '\n';
	Table << std::setw(22) << std::left << " Total Credits Spent" << '|' << std::setw(18) << std::right << TotalCreditSpent << '\n';
	Table << std::setw(22) << std::left << " Total Credits Won" << '|' << std::setw(18) << std::right << TotalCreditsWon << '\n';
	Table << std::setw(22) << std::left << " Bonus Credits Won" << '|' << std::setw(18) << std::right << BonusCreditsWon << '\n';
	Table << std::setw(22) << std::left << " BaseGame Credits Won" << '|' << std::setw(18) << std::right << BaseGameCreditsWon << '\n';
	Table << std::setw(22) << std::left << " Return to Player" << '|' << std::setw(18) << std::right << ReturnToPlayer << '\n';
	Table << std::setw(22) << std::left << " Bonus hit freq." << '|' << std::setw(18) << std::right << BonusGameHitFrequency << '\n';
	
	Table << "\n Symbol Frequency: \n\n";
	Table << std::setw(10) << std::right << "Symbol" << " | " << std::setw(10) << std::left << "Frequency" << " | " << std::setw(14) << std::right << "Probability" << '\n';
	Table << "\n* ----------------------------------------- *" << '\n';
	for (const std::pair<Symbol, int32_t> Pair : FrequencyTable)
	{
		const std::string StringSymbol	= SymbolManager::ToString(Pair.first);
		const int32_t Frequency			= Pair.second;

		Table	<< std::setw(10) << std::right << StringSymbol << " | "
				<< std::setw(10) << std::left << Frequency << " | "
				<< std::setw(14) << std::right << Frequency / static_cast<float>(GamesPlayed) << '\n';
	}

	Table << "\n* ----------------------------------------- *" << '\n';
	std::cout << Table.str() << std::endl;
}

void Slots::SlotmachineResults::UpdateFrequency(const Symbol Symbol)
{
	const bool bExists = FrequencyTable.find(Symbol) != FrequencyTable.end();
	if (bExists)
	{
		const int32_t Value = FrequencyTable[Symbol];
		FrequencyTable[Symbol] = Value + 1;

		return;
	}

	const std::pair<Slots::Symbol, int32_t> Pair(Symbol, 1);
	FrequencyTable.insert(Pair);
}

void Slots::SlotmachineResults::UpdateFrequency(const std::vector<Symbol>& PlayResult)
{
	for (const Symbol& Symbol : PlayResult)
	{
		UpdateFrequency(Symbol);
	}
}

