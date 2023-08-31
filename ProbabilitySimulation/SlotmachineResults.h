#pragma once
#include <cstdint>

namespace Slots
{
	class SlotmachineResults
	{
	public:
		SlotmachineResults();
		virtual ~SlotmachineResults();



	public:
		int32_t	GamesPlayed		= 0;
		int32_t GamesWon		= 0;
		int32_t Bonuses			= 0;
		int32_t AllWildCount	= 0;
		double CreditSpent		= 0.0;
		double CreditsWon		= 0.0;
	};
};