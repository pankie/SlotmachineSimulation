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

		const float Play(const float TotalBet);

		void InitializeWeightedCoins(const std::vector<Slots::Coin> Coins);

	private:
		struct Entry
		{
			Entry(Slots::Coin InCoin, int32_t InWeight) : Coin(InCoin), AccumulatedWeight(InWeight) {}
			int32_t AccumulatedWeight = 0;
			const Slots::Coin Coin;
		};

		void AddEntry(const Slots::Coin& Coin);
		const Slots::Coin GetRandomCoin() const;



	private:
		int32_t AccumulatedWeight = 0;
		std::vector<Entry> Entries;
	};
}
