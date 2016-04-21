#pragma once
#include <vector>

namespace util
{
	namespace Combinations
	{
		using CombinationsCollection = std::vector<std::vector<int>>;

		extern const int MaxElementsCount;

		const CombinationsCollection& Of(int elementsCount, int subsetSize);

		size_t Count(int elementsCount, int subsetSize);
	};

	namespace CombinationsWithRepetitions
	{
		using CombinationsCollection = std::vector<std::vector<int>>;

		extern const int MaxElementsCount;

		const CombinationsCollection& Of(int elementsCount, int subsetSize);

		size_t Count(int elementsCount, int subsetSize);
	};
}
