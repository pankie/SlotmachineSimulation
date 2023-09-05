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
	std::ostringstream Table;
	Table << "Slot machine results:" << '\n';
	Table << std::setw(18) << std::left << "Bonuses" << '|' << std::setw(8) << std::right << Bonuses << '\n';
	Table << std::setw(18) << std::left << "Wild attained" << '|' << std::setw(8) << std::right << AllWildCount << '\n';
	Table << std::setw(18) << std::left << "Games Played" << '|' << std::setw(8) << std::right << GamesPlayed << '\n';
	Table << std::setw(18) << std::left << "Games Won" << '|' << std::setw(8) << std::right << GamesWon << '\n';
	Table << std::setw(18) << std::left << "Games Lost" << '|' << std::setw(8) << std::right << (GamesPlayed - GamesWon) << '\n';
	Table << std::setw(18) << std::left << "Credits Spent" << '|' << std::setw(8) << std::right << CreditSpent << '\n';
	Table << std::setw(18) << std::left << "Credits Won" << '|' << std::setw(8) << std::right << TotalCreditsWon << '\n';
	Table << std::setw(18) << std::left << "BonusCredits Won" << '|' << std::setw(8) << std::right << BonusCreditsWon << '\n';
	std::cout << Table.str() << std::endl;
}

