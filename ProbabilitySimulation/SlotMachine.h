#pragma once

#include "SlotmachineResults.h"
#include "BonusGame.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Slots
{
	enum Symbol;
	class Reel;

	class SlotMachine
	{
	public:
		SlotMachine(const int32_t Rows = 3);
		virtual ~SlotMachine();

		void AddReel(const Reel& Reel);
		void SetPaylines(const std::vector<int32_t, std::allocator<int32_t>>& Paylines, const int32_t TotalPaylines);
		void SetBonusGame(const BonusGame& BonusGame);
		void Play();
		void Display(const std::vector<Symbol>& PlayResult) const;
		SlotmachineResults GetSlotmachineResults() const;
	private:
		void CalculateWins(const std::vector<Symbol>& Result);
		float GetPayout(const std::unordered_map<Symbol, int32_t>& FrequencyTable);

		void InitializePayTable();
	private:
		std::vector<Reel> Reels;
		std::vector<int32_t> Paylines;
		std::unique_ptr<BonusGame> BonusGamePtr;

		const int32_t Rows;
		int32_t TotalPaylines;

		std::unordered_map<Symbol, float> PayTable;

		SlotmachineResults SlotmachineResults;
	};
}