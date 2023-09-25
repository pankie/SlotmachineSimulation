#pragma once
#include "SymbolManager.h"
#include <cstdint>
#include <unordered_map>

namespace Slots
{
	class SlotmachineResults
	{
	public:
		SlotmachineResults();
		virtual ~SlotmachineResults();
		void Display() const;
		void UpdateFrequency(const std::vector<Symbol>& PlayResult);
		void UpdateFrequency(const Symbol Symbol);

	public:
		int32_t	GamesPlayed			= 0;
		int32_t GamesWon			= 0;
		float TotalCreditSpent		= 0.0f;
		float BonusCreditsWon		= 0.0f;
		float BaseGameCreditsWon	= 0.0f;

	private:
		std::unordered_map<Symbol, uint32_t> FrequencyTable;
	};
};