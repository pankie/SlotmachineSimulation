#include <random>
#include <chrono>
#include "RandomGenerator.h"

const int32_t RandomGenerator::UniformRandom(const int32_t Min, const int32_t Max)
{
	static std::random_device RandomDevice;
	static std::mt19937 Generator(RandomDevice());
	std::uniform_int_distribution<int32_t> Distribution(Min, Max);

	return Distribution(Generator);
}