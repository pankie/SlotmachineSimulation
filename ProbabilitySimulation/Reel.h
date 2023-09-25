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
		std::vector<Symbol> GetResult(const int32_t Rows = 3) const;

		void Print() const;
	private:
		std::vector<Symbol> ReelSymbols;
		int32_t	ReelIndex = 0;
	};
}
