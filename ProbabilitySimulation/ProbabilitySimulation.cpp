#include <iostream>

#include <vector>
#include <cstdint>
#include <fstream>
#include <chrono>
#include "SymbolManager.h"
#include "SlotMachine.h"
#include "Reel.h"
#include "BonusGame.h"

int32_t CopyPaylines(const std::string FilePath, std::vector<int32_t, std::allocator<int32_t>>& OutPaylines)
{
	std::fstream File;
	File.open(FilePath, std::ios::in);
	int32_t Index = 0;
	int32_t TotalPaylines = 0;
	if (File.is_open())
	{
		std::string Content;
		while (std::getline(File, Content))
		{
			if (!Content.compare("-"))
			{
				TotalPaylines = TotalPaylines + 1;
				continue;
			}

			for (const char Character : Content)
			{
				if (isspace(Character))
				{
					continue;
				}

				const int32_t ToInteger = static_cast<int32_t>(atoi(&Character));
				OutPaylines[Index]		= ToInteger;
				Index					= Index + 1;
			}
		}
	}

	return TotalPaylines;
}

void CopyDataToReel(const std::string FilePath, Slots::Reel& OutReel)
{
	std::vector<Slots::Symbol> Data;
	std::fstream File;
	File.open(FilePath, std::ios::in);
	if (File.is_open())
	{
		std::string Content;
		while (std::getline(File, Content))
		{
			Slots::Symbol Symbol = Slots::SymbolManager::SymbolTable.at(Content);
			Data.push_back(Symbol);
		}

		File.close();
	}

	OutReel.Initialize(Data);
}

void CopyCoinWeightTable(const std::string FilePath, Slots::BonusGame& OutBonusGame)
{
	std::vector<Slots::Coin> Coins;
	std::fstream File;
	File.open(FilePath, std::ios::in);
	if (File.is_open())
	{
		std::string Content;
		while (std::getline(File, Content))
		{
			std::string StrValue; 
			std::string StrWeight;
			bool bNext = false;
			for (const char Character : Content)
			{
				const bool bFillNextValue = Character == ',';
				if (bFillNextValue)
				{
					bNext = true;
					continue;
				}

				if (!bNext)
				{
					StrValue += Character;
				}
				else
				{
					StrWeight += Character;
				}
			}

			const int32_t Value = static_cast<int32_t>(stoi(StrValue));
			const int32_t Weight = static_cast<int32_t>(stoi(StrWeight));
			Slots::Coin Coin(Value, Weight);
			// std::cout << Coin.ToString() << '\n';
			Coins.push_back(Coin);
		}
	}

	OutBonusGame.InitializeWeightedCoins(Coins);
}

int main()
{
    std::cout << "Probability Simulation for a slot machine!\n";

	const int32_t Rows = 3;
	Slots::SlotMachine SlotMachine(Rows);

	// ------------------------------------------------------------------
	// Initialize the slot machine
	{
		// Initialize the reels
		Slots::Reel Reel0;
		CopyDataToReel("ReelData0.txt", Reel0);
		Slots::Reel Reel1;
		CopyDataToReel("ReelData1.txt", Reel1);
		Slots::Reel Reel2;
		CopyDataToReel("ReelData2.txt", Reel2);

		SlotMachine.AddReel(Reel0);
		SlotMachine.AddReel(Reel1);
		SlotMachine.AddReel(Reel2);

		// Initialize the paylines
		std::vector<int32_t, std::allocator<int32_t>> Paylines(5 * 9);
		const int32_t TotalPaylines = CopyPaylines("Paylines3x3.txt", Paylines);
		SlotMachine.SetPaylines(Paylines, TotalPaylines);

		// Initialize the bonus game
		Slots::BonusGame BonusGame;
		CopyCoinWeightTable("CoinWeightTable.txt", BonusGame);
		
		SlotMachine.SetBonusGame(BonusGame);
	}

	auto Since = [](std::chrono::time_point<std::chrono::steady_clock> const& Start)
	{
		std::chrono::steady_clock::time_point End = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::seconds>(End - Start);
	};

	const std::chrono::time_point<std::chrono::steady_clock> Start = std::chrono::steady_clock::now();
	// Play the game a couple of times!
	const int32_t BigNumber = 100;
	for (int32_t i = 0; i < BigNumber; i++)
	{
		SlotMachine.Play();
	}
	
	std::cout << "Time lapsed (seconds): " << Since(Start).count() << "\n";
	const Slots::SlotmachineResults Results = SlotMachine.GetSlotmachineResults();
	Results.Display();
}