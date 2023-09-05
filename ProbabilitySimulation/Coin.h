#pragma once

#include <cstdint>
#include <string>

namespace Slots
{
	class Coin
	{
	public:
		Coin(const int32_t Value, const int32_t Weight);
		Coin(const Coin& Other);
		virtual ~Coin();

	public:
		const std::string ToString();

	public:
		const int32_t Value;
		const int32_t Weight;
		const double Probability;
	};
}