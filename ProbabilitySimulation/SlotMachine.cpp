
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

void Slots::SlotMachine::SetPaylines(const std::vector<int32_t, std::allocator<int32_t>>& Paylines, const int32_t TotalPaylines)
{
	this->TotalPaylines = TotalPaylines;
	this->Paylines		= Paylines;
}

void Slots::SlotMachine::Play()
{
	int32_t ResultIndex			= 0;
	const int32_t TotalColumns	= static_cast<int32_t>(Reels.size());
	std::vector<Slots::Symbol> PlayResult = std::vector<Slots::Symbol>(TotalColumns * Rows);
	for (Slots::Reel& Reel : Reels)
	{
		Reel.Spin();
		const std::vector<Slots::Symbol> ReelResult = Reel.GetResult(Rows);
		for (int32_t i = 0; i < ReelResult.size(); i++)
		{
			const Slots::Symbol Symbol	= ReelResult[i];
			PlayResult[ResultIndex]		= Symbol;
			ResultIndex					= ResultIndex + 1;
		}
	}

	CalculateWins(PlayResult);
	SlotmachineResults.GamesPlayed = SlotmachineResults.GamesPlayed + 1;
	SlotmachineResults.CreditSpent = SlotmachineResults.CreditSpent + 10.0f;
}

void Slots::SlotMachine::Display(const std::vector<Slots::Symbol>& PlayResult)
{
	const int32_t TotalColumns = static_cast<int32_t>(Reels.size());
	for (int32_t i = 0; i < Rows; i++)
	{
		for (int32_t j = 0; j < TotalColumns; j++)
		{
			const Slots::Symbol Symbol		= PlayResult[i * TotalColumns + j];
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

void Slots::SlotMachine::CalculateWins(const std::vector<Slots::Symbol>& Result)
{
	// Assuming there is no paylines of different sizes e.g. 
	// 3x3 payline matrices contains only fix size of 3 ones each column and not 2 or 1 ones. 
	const int32_t PaylineSize					= static_cast<int32_t>(Paylines.size()) / TotalPaylines;
	const int32_t TotalColumns					= static_cast<int32_t>(Reels.size());
	// const std::vector<Slots::Symbol>& Result	= GetResult();

	for (size_t i = 0; i < TotalPaylines; i++)
	{
		// Stores the amount of occurring symbols in a map
		// corresponding to the payline positions
		std::unordered_map<Slots::Symbol, int32_t> FrequencyTable;
		for (size_t j = 0; j < PaylineSize; j++)
		{
			const size_t ElementIndex = PaylineSize * i + j;

			// Paylines[i] contains 0 or 1
			if (!Paylines[ElementIndex])
			{
				continue;
			}

			Slots::Symbol Symbol	= Result[j];
			const bool bNotInTable	= FrequencyTable.find(Symbol) == FrequencyTable.end();
			if (bNotInTable)
			{
				auto Pair = std::pair<Slots::Symbol, int32_t>(Symbol, 1);
				FrequencyTable.insert(Pair);
				continue;
			}

			// If we reach this point in code, then we just increase the frequency for this symbol
			int32_t& Value	= FrequencyTable.at(Symbol);
			Value			= Value + 1;
		}

		const float Total = GetPayout(FrequencyTable);
		if (Total > 0.001f)
		{
			// std::cout << "----------------------------------" << std::endl;
			// Display(Result);
			SlotmachineResults.GamesWon		= SlotmachineResults.GamesWon + 1;
			SlotmachineResults.CreditsWon	= SlotmachineResults.CreditsWon + Total;
			// std::cout << "total wins: " << SlotmachineResults.GamesWon << std::endl;
		}
	}
}

const float Slots::SlotMachine::GetPayout(const std::unordered_map<Slots::Symbol, int32_t>& FrequencyTable)
{
	// For the case when we have all distinct, then
	// SymbolFrequency = 1

	// Find the most frequent symbol and how often it is occurring
	Slots::Symbol MostFrequentSymbol = NONE;
	int32_t SymbolFrequency = 0;
	for (const std::pair<Slots::Symbol, int32_t>& Pair : FrequencyTable)
	{
		if (Pair.second > SymbolFrequency)
		{
			MostFrequentSymbol	= Pair.first;
			SymbolFrequency		= Pair.second;
		}
	}

	// All symbols are distinct
	if (SymbolFrequency == 1)
	{
		return 0.0f;
	}

	auto GetFrequency = [&FrequencyTable](const Slots::Symbol& Symbol){
		const bool bNotFound = FrequencyTable.find(Symbol) == FrequencyTable.end();
		if (bNotFound)
		{
			return 0;
		}

		const int32_t Frequency = FrequencyTable.at(Symbol);
		return Frequency;
	};

	// W1 symbols can substitute all symbols (except the bonus B1),
	// e.g. if the payline is generating the following symbols
	//				L2 W1 L2
	// then W1 will be substituted as an L2 symbol.
	const int32_t WildCards		= GetFrequency(Slots::Symbol::W1);
	const int32_t TotalReels	= static_cast<int32_t>(Reels.size());
	//if (WildCards + SymbolFrequency != TotalReels)
	//{
	//	// If we reach this part of the code, we haven't won anything.
	//	return 0.0f;
	//}

	const bool bSameSymbolAllReels = SymbolFrequency == TotalReels;
	if (bSameSymbolAllReels)
	{
		const bool bAllWild = MostFrequentSymbol == Slots::Symbol::W1;
		if (bAllWild)
		{
			// Maximum payment!
			SlotmachineResults.AllWildCount = SlotmachineResults.AllWildCount + 1;
		}

		const bool bAllBonuses = MostFrequentSymbol == Slots::Symbol::B1;
		if (bAllBonuses)
		{
			// Todo: Trigger bonus round
			// const float Payout = BonusGame.Play();
			SlotmachineResults.Bonuses = SlotmachineResults.Bonuses + 1;
			return 0.0f;
		}

		return PayTable[MostFrequentSymbol];
	}

	// 3 in a row
	if (SymbolFrequency == 2 && WildCards != 0)
	{
		if (WildCards == 1)
		{
			if (MostFrequentSymbol == Symbol::B1)
			{
				return 0.0f;
			}

			return PayTable[MostFrequentSymbol];
		}

		// In the case the most frequent symbol is W1, for example as the sequence
		// W1 W1 H2
		// then the pay out should be still count as 3 x H2 according to the pay table.
		if (WildCards == 2)
		{
			for (std::pair<Slots::Symbol, int32_t> Pair : FrequencyTable)
			{
				if (Pair.first != Slots::Symbol::W1)
				{
					MostFrequentSymbol = Pair.first;
					break;
				}
			}

			return PayTable[MostFrequentSymbol];
		}
	}

	//const std::string StrSymbol = SymbolManager::ToString(MostFrequentSymbol);
	//std::cout << "Most frequent symbol is : " << StrSymbol << '\n';
	return 0.0f;
}

void Slots::SlotMachine::InitializePayTable()
{
	// Todo: Make this into a .txt file instead and read the data to the
	// slot machine.
	std::pair<Slots::Symbol, float> Pair;
	Pair.first	= Slots::Symbol::W1;
	Pair.second = 2000.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::H1;
	Pair.second = 800.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::H2;
	Pair.second = 400.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::H3;
	Pair.second = 80.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::L1;
	Pair.second = 60.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::L2;
	Pair.second = 20.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::L3;
	Pair.second = 16.0f;
	PayTable.insert(Pair);

	Pair.first	= Slots::Symbol::L4;
	Pair.second = 12.0f;
	PayTable.insert(Pair);
}

