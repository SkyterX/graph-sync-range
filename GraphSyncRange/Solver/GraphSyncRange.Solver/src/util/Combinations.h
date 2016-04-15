#pragma once
#include <vector>

namespace util
{
	class Combinations {
		using CombinationsCollection = std::vector<std::vector<int>>;
		using CollectionOfCombinations = std::vector<std::vector<CombinationsCollection>>;

		static CollectionOfCombinations combinations;
		static CombinationsCollection GenerateCombinations(int elementsCount, int subsetSize);
		static CollectionOfCombinations GenerateAllCombinations(int maxElementsCount);

	public:
		static const int MaxElementsCount;

		static const CombinationsCollection& Of(int elementsCount, int subsetSize);

		static size_t Count(int elementsCount, int subsetSize);
	};

	class CombinationsWithRepetitions {
		using CombinationsCollection = std::vector<std::vector<int>>;
		using CollectionOfCombinations = std::vector<std::vector<CombinationsCollection>>;

		static CollectionOfCombinations combinationsWithRepetitions;
		static CombinationsCollection GenerateCombinationsWithRepetitions(int elementsCount, int subsetSize);
		static CollectionOfCombinations GenerateAllCombinationsWithRepetitions(int maxElementsCount);

	public:
		static const int MaxElementsCount;

		static const CombinationsCollection& Of(int elementsCount, int subsetSize);

		static size_t Count(int elementsCount, int subsetSize);
	};
}
