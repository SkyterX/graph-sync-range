#include <util/Permutations.h>
#include <map>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

namespace util
{
	namespace Permutation
	{
		extern const int Permutation::MaxElementsCount = 7;

		namespace
		{
			struct PermutationStorage {
				using PermutationCollection = vector<vector<int>>;
				using CollectionsOfPermutations = vector<PermutationCollection>;

				CollectionsOfPermutations permutations;
				vector<map<vector<int>, IdType>> permutationIds;

				PermutationStorage() {
					GenerateAllPermutations(MaxElementsCount);
				}

				void GenerateAllPermutations(int maxElementsCount) {
					permutations.resize(maxElementsCount + 1);
					permutationIds.resize(maxElementsCount + 1);
					for (int elementsCount = 0; elementsCount <= maxElementsCount; ++elementsCount) {
						GeneratePermutations(elementsCount);
					}
				}

				void GeneratePermutations(int elementsCount) {
					if (permutations.size() < elementsCount + 1) {
						permutations.resize(elementsCount + 1);
						permutationIds.resize(elementsCount + 1);
					}

					vector<int> permutation(elementsCount);
					iota(permutation.begin(), permutation.end(), 0);

					permutations[elementsCount].reserve(Factorial::Of(elementsCount));
					int id = 0;
					do {
						permutations[elementsCount].push_back(permutation);
						permutationIds[elementsCount][permutation] = id;
						++id;
					}
					while (next_permutation(permutation.begin(), permutation.end()));
				}

				const vector<int>& ById(int pSize, IdType id) {
					assert(0 < pSize && pSize < permutations.size());
					assert(0 <= id && id < permutations[pSize].size());
					return permutations[pSize][id];
				}

				IdType GetId(const vector<int>& p) {
					assert(0 < p.size() && p.size() < permutationIds.size());
					return permutationIds[p.size()][p];
				}
			} Storage;
		}

		void GeneratePermutations(int pSize) {
			Storage.GeneratePermutations(pSize);
		}

		const vector<int>& ById(int pSize, int id) {
			return Storage.ById(pSize, id);
		}

		IdType GetId(const vector<int>& p) {
			return Storage.GetId(p);
		}
	}
}

namespace util
{
	LazyPermutation::LazyPermutation(IdType permutation_id): permutationId(permutation_id) {}

	LazyPermutation::LazyPermutation(const vector<int>& p) {
		permutationId = Permutation::GetId(p);
		return;
	}

	const vector<int>& LazyPermutation::GetPermutation(int pSize) const {
		return Permutation::ById(pSize, permutationId);
	}

	bool LazyPermutation::NextPermutation(int pSize) {
		permutationId = (permutationId + 1) % Factorial::Of(pSize);
		return permutationId != 0;
	}

	LazyPermutation::IdType LazyPermutation::PermutationsCount(int pSize) {
		return Factorial::Of(pSize);
	}
}
