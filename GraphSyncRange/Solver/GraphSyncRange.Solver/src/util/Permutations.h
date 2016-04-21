#pragma once
#include <vector>

#include <util/Factorial.h>

namespace util
{
	namespace Permutation
	{
		using IdType = Factorial::IdType;

		extern const int MaxElementsCount;
		void GeneratePermutations(int pSize);

		const std::vector<int>& ById(int pSize, int id);

		IdType GetId(const std::vector<int>& p);
	}


	struct LazyPermutation {
		using IdType = Factorial::IdType;

		IdType permutationId;

		explicit LazyPermutation(IdType permutation_id = 0);

		explicit LazyPermutation(const std::vector<int>& p);

		const std::vector<int>& GetPermutation(int pSize) const;

		bool NextPermutation(int pSize);

		static IdType PermutationsCount(int pSize);
	};
}
