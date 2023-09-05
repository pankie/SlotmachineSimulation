#pragma once

#include <cstdint>
class RandomGenerator
{
public:
	static const int32_t UniformRandom(const int32_t Min, const int32_t Max);  // Min <= x <= Max (Closed interval)
	static const double UniformRandom(const float Min, const float Max); // Min <= x < Max  (Clopen interval)
};
