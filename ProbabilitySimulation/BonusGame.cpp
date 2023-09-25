#include "BonusGame.h"
#include "RandomGenerator.h"
#include <iostream>
#include <sstream>
#include <iomanip>


Slots::BonusGame::BonusGame()
{
}

Slots::BonusGame::~BonusGame()
{
}

void Slots::BonusGame::Simulate() const
{
	std::cout << "\nSimulating bonus game ...  " << '\n';
	
	const int32_t Sims = 10000000;
	double ValueSum = 0.0;
	double DieSum = 0.0;

	for (int32_t i = 0; i < Sims; i++)
	{
		ValueSum += GetRandomCoin().Value;
		const int32_t DiceValue = RandomGenerator::UniformRandom(1, 6);
		DieSum += DiceValue;
	}

	double ValueAvg = 0.0;
	for (const Entry& Entry : Entries)
	{
		const Coin Coin = Entry.Coin;
		ValueAvg += Coin.Value * Coin.Probability;
	}

	double DieAvg = 0.0;
	for (int32_t i = 1; i < 7; i++)
	{
		constexpr double Probability = 1.0 / 6.0;
		DieAvg += Probability * i;
	}

	const double ValueEst	= ValueSum / Sims;
	const double DieEst		= DieSum / Sims;

	std::stringstream StringStream;
	StringStream << "* ------------------------ Bonus game simulation ------------------------ *\n";
	StringStream << "\n Weighted coin toss and Dice comparisons: \n\n";
	StringStream << std::setw(30) << std::left << "  True die average" << '|' << std::setw(12) << std::right << DieAvg << '\n';
	StringStream << std::setw(30) << std::left << "  Die simulated" << '|' << std::setw(12) << std::right << DieEst << '\n';
	StringStream << std::setw(30) << std::left << "  True Coin value average" << '|' << std::setw(12) << std::right << ValueAvg << '\n';
	StringStream << std::setw(30) << std::left << "  Coin value simulated" << '|' << std::setw(12) << std::right << ValueEst << '\n';
	StringStream << std::setw(30) << std::left << "  Bonus game simulated. Win" << '|' << std::setw(12) << std::right << ValueEst * DieEst * 10.0f  << '\n';
	
	StringStream << "\n Error size: \n\n";
	StringStream << std::setw(30) << std::left << "  |Avg. Die - Sim. Die|" << '|' << std::setw(12) << std::right << abs(DieAvg - DieEst) << '\n';
	StringStream << std::setw(30) << std::left << "  |Avg. Value - Sim. Value| " << '|' << std::setw(12) << std::right << abs(ValueAvg - ValueEst) << '\n';
	StringStream << "\n* ------------------------------------------------------------------------ *\n";
	std::cout << StringStream.str();
}

float Slots::BonusGame::Play(const float TotalBet) const
{
	const Coin Coin	= GetRandomCoin();
	const int32_t CoinValue = Coin.Value;
	const int32_t DiceValue = RandomGenerator::UniformRandom(1, 6);

	return TotalBet * DiceValue * CoinValue;
}

void Slots::BonusGame::InitializeWeightedCoins(const std::vector<Coin>& Coins)
{
	for (const Coin& Coin : Coins)
	{
		AddEntry(Coin);
	}
}

void Slots::BonusGame::AddEntry(const Coin& Coin)
{
	AccumulatedWeight += Coin.Weight;
	Entry Entry(Coin, AccumulatedWeight);
	Entries.push_back(Entry);
}

Slots::Coin Slots::BonusGame::GetRandomCoin() const
{
	const double Value = RandomGenerator::UniformRandom(0.0f, 1.0f) * AccumulatedWeight;
	for (const Entry& Entry : Entries)
	{
		if (Entry.AccumulatedWeight > Value)
		{
			return Entry.Coin;
		}
	}

	// We shouldn't be able to reach this part of the code.
	std::cout << "Warning: The BonusGame::GetRandomCoin() returned non-existent coin, this shouldn't happen." << std::endl;
	Coin DefaultCoin(-1, -1);
	return DefaultCoin; // Todo: Return a nullptr instead?
}

