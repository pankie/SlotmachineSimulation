#pragma once

#include "SlotmachineResults.h"
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace Slots
{
	enum Symbol;
	class Reel;

	class SlotMachine
	{
	public:
		SlotMachine(const int32_t Rows = 3);
		virtual ~SlotMachine();
		void AddReel(const Slots::Reel& Reel);
		void SetPaylines(const std::vector<int32_t, std::allocator<int32_t>>& Paylines, const int32_t TotalPaylines);
		void Play();
		void Display(const std::vector<Slots::Symbol>& PlayResult);
		Slots::SlotmachineResults GetSlotmachineResults() const;
	private:
		void CalculateWins(const std::vector<Slots::Symbol>& Result);
		const float GetPayout(const std::unordered_map<Slots::Symbol, int32_t>& FrequencyTable);

		void InitializePayTable();
	private:
		std::vector<Reel> Reels;
		// std::vector<Slots::Symbol> PlayResult;
		std::vector<int32_t> Paylines;

		const int32_t Rows;
		int32_t TotalPaylines;

		std::unordered_map<Slots::Symbol, float> PayTable;
		
		Slots::SlotmachineResults SlotmachineResults;
	};
}