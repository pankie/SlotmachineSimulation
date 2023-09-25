#pragma once

#include <cstdint>
#include "Coin.h"
#include <vector>

namespace Slots
{
	class BonusGame
	{
	public:
		BonusGame();
		virtual ~BonusGame();

		void Simulate() const;

		float Play(const float TotalBet) const;

		void InitializeWeightedCoins(const std::vector<Coin>& Coins);

	private:
		struct Entry
		{
			Entry(Coin InCoin, int32_t InWeight) : Coin(InCoin), AccumulatedWeight(InWeight) {}
			int32_t AccumulatedWeight = 0;
			const Coin Coin;
		};

		void AddEntry(const Coin& Coin);
		Coin GetRandomCoin() const;



	private:
		int32_t AccumulatedWeight = 0;
		std::vector<Entry> Entries;
	};
}
