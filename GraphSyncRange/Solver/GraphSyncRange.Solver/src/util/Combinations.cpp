#include <util/Combinations.h>
#include <util/Factorial.h>
#include <algorithm>
#include <cassert>

using namespace std;

namespace util
{
	const int Combinations::MaxElementsCount = 10;
	Combinations::CollectionOfCombinations Combinations::combinations = GenerateAllCombinations(MaxElementsCount);

	Combinations::CombinationsCollection Combinations::GenerateCombinations(int elementsCount, int subsetSize) {
		CombinationsCollection combinationsNK;

		vector<bool> v(elementsCount, false);
		fill_n(v.begin(), subsetSize, true);

		do {
			combinationsNK.push_back({});
			for (int i = 0; i < v.size(); ++i) {
				if (v[i])
					combinationsNK.back().push_back(i);
			}
		}
		while (prev_permutation(v.begin(), v.end()));

		return combinationsNK;
	}

	Combinations::CollectionOfCombinations Combinations::GenerateAllCombinations(int maxElementsCount) {
		vector<vector<vector<vector<int>>>> allCombinations;

		for (int elementsCount = 0; elementsCount < maxElementsCount; ++elementsCount) {
			allCombinations.push_back({});
			for (int subsetSize = 0; subsetSize <= elementsCount; ++subsetSize) {
				allCombinations[elementsCount].push_back(GenerateCombinations(elementsCount, subsetSize));
			}
		}

		return allCombinations;
	}

	const Combinations::CombinationsCollection& Combinations::Of(int elementsCount, int subsetSize) {
		assert(0 <= elementsCount && elementsCount < combinations.size());
		assert(0 <= subsetSize && subsetSize <= elementsCount);
		return combinations[elementsCount][subsetSize];
	}

	size_t Combinations::Count(int elementsCount, int subsetSize) {
		size_t result = 1;
		for (int i = 1; i <= subsetSize; ++i) {
			result *= elementsCount - subsetSize + i;
			result /= i;
		}
		return result;
	}
}

namespace util
{
	const int CombinationsWithRepetitions::MaxElementsCount = 15;
	CombinationsWithRepetitions::CollectionOfCombinations CombinationsWithRepetitions::combinationsWithRepetitions = GenerateAllCombinationsWithRepetitions(MaxElementsCount);

	CombinationsWithRepetitions::CombinationsCollection CombinationsWithRepetitions::GenerateCombinationsWithRepetitions(int elementsCount, int subsetSize) {
		CombinationsCollection combinationsNK;

		vector<bool> v(elementsCount + subsetSize - 1, false);
		fill_n(v.begin() + subsetSize, elementsCount - 1, true);

		do {
			combinationsNK.push_back({});
			int vertex = 0;
			for (int i = 0; i < v.size(); ++i) {
				if (v[i])
					++vertex;
				else
					combinationsNK.back().push_back(vertex);
			}
		}
		while (next_permutation(v.begin(), v.end()));

		return combinationsNK;
	}

	CombinationsWithRepetitions::CollectionOfCombinations CombinationsWithRepetitions::GenerateAllCombinationsWithRepetitions(int maxElementsCount) {
		vector<vector<vector<vector<int>>>> allCombinationsWithRepetitions;

		allCombinationsWithRepetitions.push_back({{{}}});
		for (int subsetSize = 1; subsetSize < maxElementsCount; ++subsetSize)
			allCombinationsWithRepetitions[0].push_back({});
		for (int elementsCount = 1; elementsCount < maxElementsCount; ++elementsCount) {
			allCombinationsWithRepetitions.push_back({});
			for (int subsetSize = 0; elementsCount + subsetSize < maxElementsCount; ++subsetSize) {
				allCombinationsWithRepetitions[elementsCount].push_back(GenerateCombinationsWithRepetitions(elementsCount, subsetSize));
			}
		}

		return allCombinationsWithRepetitions;
	}

	const CombinationsWithRepetitions::CombinationsCollection& CombinationsWithRepetitions::Of(int elementsCount, int subsetSize) {
		assert(0 <= subsetSize);
		assert(0 <= elementsCount && elementsCount < combinationsWithRepetitions.size());
		assert(elementsCount + subsetSize < MaxElementsCount);
		return combinationsWithRepetitions[elementsCount][subsetSize];
	}

	size_t CombinationsWithRepetitions::Count(int elementsCount, int subsetSize) {
		return Combinations::Count(elementsCount + subsetSize - 1, subsetSize);
	}
}
