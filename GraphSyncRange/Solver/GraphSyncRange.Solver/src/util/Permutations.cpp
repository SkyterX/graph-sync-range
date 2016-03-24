#include <util/Permutations.h>
#include <algorithm>

using namespace std;

namespace util
{
	vector<vector<vector<int>>> Permutation::permutations;
	vector<map<vector<int>, Permutation::IdType>> Permutation::permutationIds;

	void Permutation::Generate(int pSize) {
		if (permutations.size() < pSize + 1) {
			permutations.resize(pSize + 1);
			permutationIds.resize(pSize + 1);
		}

		vector<int> permutation(pSize);
		for (int i = 0; i < pSize; ++i) {
			permutation[i] = i;
		}

		permutations[pSize].reserve(Factorial::Of(pSize));
		int id = 0;
		do {
			permutations[pSize].push_back(permutation);
			permutationIds[pSize][permutation] = id;
			++id;
		}
		while (next_permutation(permutation.begin(), permutation.end()));
	}

	const vector<int>& Permutation::ById(int pSize, int id) {
		return permutations[pSize][id];
	}

	Permutation::IdType Permutation::GetId(const vector<int>& p) {
		return permutationIds[p.size()][p];
	}
}

namespace util
{
	LazyPermutation::LazyPermutation(IdType permutation_id): permutationId(permutation_id) {}

	LazyPermutation::LazyPermutation(const vector<int>& p) {
		permutationId = Permutation::GetId(p);
		return;
	}

	vector<int> LazyPermutation::GetPermutation(int pSize) const {
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
