
#include "SlotMachine.h"
#include "Reel.h"
#include <iostream>
#include <chrono>

Slots::SlotMachine::SlotMachine(const int32_t Rows)
	: Rows(Rows)
{
	InitializePayTable();
}

Slots::SlotMachine::~SlotMachine()
{
}

void Slots::SlotMachine::AddReel(const Reel& Reel)
{
	Reels.push_back(Reel);
}

void Slots::SlotMachine::SetPaylines(const std::vector<int32_t>& Paylines, const int32_t TotalPaylines)
{
	this->TotalPaylines = TotalPaylines;
	this->Paylines		= Paylines;
}

void Slots::SlotMachine::SetBonusGame(const BonusGame& BonusGame)
{
	BonusGamePtr = std::make_unique<Slots::BonusGame>(BonusGame);
}

void Slots::SlotMachine::Play()
{
	int32_t ResultIndex				= 0;
	const int32_t TotalColumns		= static_cast<int32_t>(Reels.size());
	std::vector<Symbol> PlayResult	= std::vector<Symbol>(TotalColumns * Rows);
	for (Reel& Reel : Reels)
	{
		Reel.Spin();
		const std::vector<Symbol> ReelResult = Reel.GetResult(Rows);
		for (int32_t i = 0; i < ReelResult.size(); i++)
		{
			const Symbol Symbol			= ReelResult[i];
			PlayResult[ResultIndex]		= Symbol;
			ResultIndex					= ResultIndex + 1;
		}
	}
	
	CalculateWins(PlayResult);
	// SlotmachineResults.UpdateFrequency(PlayResult);
	SlotmachineResults.GamesPlayed		= SlotmachineResults.GamesPlayed + 1;
	SlotmachineResults.TotalCreditSpent = SlotmachineResults.TotalCreditSpent + 10.0f;
}

void Slots::SlotMachine::Display(const std::vector<Symbol>& PlayResult) const
{
	const int32_t TotalColumns = static_cast<int32_t>(Reels.size());
	for (int32_t i = 0; i < Rows; i++)
	{
		for (int32_t j = 0; j < TotalColumns; j++)
		{
			const Symbol Symbol		= PlayResult[i * TotalColumns + j];
			const std::string StringSymbol	= SymbolManager::ToString(Symbol);

			std::cout << StringSymbol << ((j + 1) == TotalColumns ? '\n' : '\t');
			std::flush(std::cout);
		}
	}
}

Slots::SlotmachineResults Slots::SlotMachine::GetSlotmachineResults() const
{
	return SlotmachineResults;
}

void Slots::SlotMachine::CalculateWins(const std::vector<Symbol>& Result)
{
	// Assuming there are no paylines of different sizes e.g. 
	// 3x3 payline matrices contains only fix size of 3 ones each column and not 2 or 1 ones. 
	const int32_t PaylineSize	= static_cast<int32_t>(Paylines.size()) / TotalPaylines;
	// const int32_t TotalColumns	= static_cast<int32_t>(Reels.size());

	bool bWon = false;

	for (size_t i = 0; i < TotalPaylines; i++)
	{
		// Stores the amount of occurring symbols in a map
		// corresponding to the payline positions
		std::unordered_map<Symbol, int32_t> FrequencyTable;
		for (size_t j = 0; j < PaylineSize; j++)
		{
			const size_t ElementIndex = PaylineSize * i + j;

			// Paylines[i] contains 0 or 1
			if (Paylines[ElementIndex] == 0)
			{
				continue;
			}

			// Handle case if Symbol was not found
			Symbol Symbol = Result[j];
			if (FrequencyTable.find(Symbol) == FrequencyTable.end())
			{
				auto Pair = std::pair(Symbol, 1);
				FrequencyTable.insert(Pair);
				continue;
			}

			// If we reach this point in code, then we just increase the frequency for this symbol
			int32_t& Value	= FrequencyTable.at(Symbol);
			Value			= Value + 1;
		}
		
		if (const float Total = GetPayout(FrequencyTable) > 1.0f)
		{
			// std::cout << "----------------------------------" << std::endl;
			// Display(Result);
			SlotmachineResults.BaseGameCreditsWon = SlotmachineResults.BaseGameCreditsWon + Total;
			bWon = true;

			// break;
		}
	}

	// Since B1 is a scatter symbol, it is allowed to occur at least once on every reel,
	// independent of the paylines, and thus lead to the bonus game.
	bool bTriggerBonusGame = true;
	for (size_t i = 0; i < 3; i++)
	{
		const Symbol S0 = Result[i];
		const Symbol S1 = Result[i + 3];
		const Symbol S2 = Result[i + 6];

		const bool bColumnContainsBonusSymbol =
			S0 == B1 ||
			S1 == B1 ||
			S2 == B1;

		// if at least one reel does not contain the bonus symbol, we will not trigger bonus game.
		if (!bColumnContainsBonusSymbol)
		{
			// No bonus.
			bTriggerBonusGame = false;
			break;
		}
	}

	if (bTriggerBonusGame)
	{
		// bWon								= true;
		const float BonusWon				= BonusGamePtr->Play(10.0f);
		SlotmachineResults.BonusCreditsWon	= SlotmachineResults.BonusCreditsWon + BonusWon;
		// SlotmachineResults.UpdateFrequency(Slots::Symbol::B1);
		// Display(Result);
	}

	if (bWon)
	{
		SlotmachineResults.GamesWon = SlotmachineResults.GamesWon + 1;
	}
}

float Slots::SlotMachine::GetPayout(const std::unordered_map<Symbol, int32_t>& FrequencyTable)
{

	// Find the most frequent symbol and how often it is occurring
	Symbol MostFrequentSymbol = NONE;
	int32_t SymbolFrequency = 0;
	for (const auto& [First, Second] : FrequencyTable)
	{
		if (Second > SymbolFrequency)
		{
			MostFrequentSymbol	= First;
			SymbolFrequency		= Second;
		}
	}

	// All symbols are distinct from each other => no win.
	if (SymbolFrequency == 1)
	{
		return 0.0f;
	}

	// 3 of same symbol
	if (SymbolFrequency == 3)
	{
		// Return payment according to the pay table
		SlotmachineResults.UpdateFrequency(MostFrequentSymbol);
		return PayTable[MostFrequentSymbol];
	}

	// At least one is Wild
	if (FrequencyTable.find(W1) != FrequencyTable.end())
	{
		for (const auto& [First, Second] : FrequencyTable)
		{
			const Symbol Symbol = First;
			if (Symbol != W1)
			{
				if (FrequencyTable.at(Symbol) == 2)
				{
					SlotmachineResults.UpdateFrequency(Symbol);
					// Payout according to pay table
					return PayTable[Symbol];
				}
				else if (FrequencyTable.at(Symbol) == 1 && FrequencyTable.at(W1) == 2)
				{
					SlotmachineResults.UpdateFrequency(Symbol);
					return PayTable[Symbol];
				}
			}
		}
	}

	return 0.0f;
}

void Slots::SlotMachine::InitializePayTable()
{
	// Todo: Make this into a .txt file instead and read the data to the
	// slot machine.
	std::pair<Symbol, float> Pair;
	Pair.first	= W1;
	Pair.second = 2000.0f;
	PayTable.insert(Pair);

	Pair.first	= H1;
	Pair.second = 800.0f;
	PayTable.insert(Pair);

	Pair.first	= H2;
	Pair.second = 400.0f;
	PayTable.insert(Pair);

	Pair.first	= H3;
	Pair.second = 80.0f;
	PayTable.insert(Pair);

	Pair.first	= L1;
	Pair.second = 60.0f;
	PayTable.insert(Pair);

	Pair.first	= L2;
	Pair.second = 20.0f;
	PayTable.insert(Pair);

	Pair.first	= L3;
	Pair.second = 16.0f;
	PayTable.insert(Pair);

	Pair.first	= L4;
	Pair.second = 12.0f;
	PayTable.insert(Pair);

	// The bonus is handled separately
	Pair.first = B1;
	Pair.second = 0.0f;
	PayTable.insert(Pair);
}

