#pragma once
#include <vector>
#include <cstdint>
#include <random>
#include "SymbolManager.h"

namespace Slots
{
	class Reel
	{
	public: 
		Reel();
		virtual ~Reel();

		void Initialize(const std::vector<Symbol>& ReelSymbols);
		void Spin();
		const std::vector<Symbol> GetResult(const int32_t Rows = 3);

		void Print();
	private:
		std::vector<Symbol> ReelSymbols;
		int32_t	ReelIndex = 0;
	};
}
