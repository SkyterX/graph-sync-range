#include <util/Combinations.h>
#include <util/Factorial.h>
#include <algorithm>
#include <cassert>

using namespace std;

namespace util
{
	namespace Combinations
	{
		const int MaxElementsCount = 10;

		namespace
		{
			using CollectionOfCombinations = vector<vector<CombinationsCollection>>;

			CombinationsCollection GenerateCombinations(int elementsCount, int subsetSize) {
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

			CollectionOfCombinations GenerateAllCombinations(int maxElementsCount) {
				vector<vector<vector<vector<int>>>> allCombinations;

				for (int elementsCount = 0; elementsCount < maxElementsCount; ++elementsCount) {
					allCombinations.push_back({});
					for (int subsetSize = 0; subsetSize <= elementsCount; ++subsetSize) {
						allCombinations[elementsCount].push_back(GenerateCombinations(elementsCount, subsetSize));
					}
				}

				return allCombinations;
			}

			CollectionOfCombinations combinations = GenerateAllCombinations(MaxElementsCount);
		}

		const CombinationsCollection& Of(int elementsCount, int subsetSize) {
			assert(0 <= elementsCount && elementsCount < combinations.size());
			assert(0 <= subsetSize && subsetSize <= elementsCount);
			return combinations[elementsCount][subsetSize];
		}

		size_t Count(int elementsCount, int subsetSize) {
			size_t result = 1;
			for (int i = 1; i <= subsetSize; ++i) {
				result *= elementsCount - subsetSize + i;
				result /= i;
			}
			return result;
		}
	}
}

namespace util
{
	namespace CombinationsWithRepetitions
	{
		using CollectionOfCombinations = vector<vector<CombinationsCollection>>;

		const int MaxElementsCount = 15;

		namespace
		{
			CombinationsCollection GenerateCombinationsWithRepetitions(int elementsCount, int subsetSize) {
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

			CollectionOfCombinations GenerateAllCombinationsWithRepetitions(int maxElementsCount) {
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

			CollectionOfCombinations combinationsWithRepetitions = GenerateAllCombinationsWithRepetitions(MaxElementsCount);
		}

		const CombinationsCollection& Of(int elementsCount, int subsetSize) {
			assert(0 <= subsetSize);
			assert(0 <= elementsCount && elementsCount < combinationsWithRepetitions.size());
			assert(elementsCount + subsetSize < MaxElementsCount);
			return combinationsWithRepetitions[elementsCount][subsetSize];
		}

		size_t Count(int elementsCount, int subsetSize) {
			return Combinations::Count(elementsCount + subsetSize - 1, subsetSize);
		}
	}
}
