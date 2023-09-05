#include "BonusGame.h"
#include "RandomGenerator.h"
#include <iostream>


Slots::BonusGame::BonusGame()
{
}

Slots::BonusGame::~BonusGame()
{
}

const float Slots::BonusGame::Play(const float TotalBet)
{
	const Slots::Coin Coin	= GetRandomCoin();
	const int32_t CoinValue = Coin.Value;
	const int32_t DiceValue = RandomGenerator::UniformRandom(1, 6);

	return TotalBet * DiceValue * CoinValue;
}

void Slots::BonusGame::InitializeWeightedCoins(const std::vector<Slots::Coin> Coins)
{
	for (const Slots::Coin& Coin : Coins)
	{
		AddEntry(Coin);
	}
}

void Slots::BonusGame::AddEntry(const Slots::Coin& Coin)
{
	AccumulatedWeight += Coin.Weight;
	BonusGame::Entry Entry(Coin, AccumulatedWeight);
	Entries.push_back(Entry);
}

const Slots::Coin Slots::BonusGame::GetRandomCoin() const
{
	const double Value = RandomGenerator::UniformRandom(0.0f, 1.0f) * AccumulatedWeight;
	for (const BonusGame::Entry& Entry : Entries)
	{
		if (Entry.AccumulatedWeight > Value)
		{
			return Entry.Coin;
		}
	}

	// We shouldn't be able to reach this part of the code.
	std::cout << "Warning: The BonusGame::GetRandomCoin() returned non-existent coin, this shouldn't happen." << std::endl;
	Slots::Coin DefaultCoin(-1, -1);
	return DefaultCoin; // Todo: Return a nullptr instead?
}

