#pragma once
#include <vector>
#include <map>

#include <util/Factorial.h>

namespace util
{
	class Permutation {
	public:
		using IdType = Factorial::IdType;

	private:

		static std::vector<std::vector<std::vector<int>>> permutations;
		static std::vector<std::map<std::vector<int>, IdType>> permutationIds;
	public:

		static void Generate(int pSize);

		static const std::vector<int>& ById(int pSize, int id);

		static IdType GetId(const std::vector<int>& p);
	};


	struct LazyPermutation {
		using IdType = Factorial::IdType;

		IdType permutationId;

		explicit LazyPermutation(IdType permutation_id = 0);

		explicit LazyPermutation(const std::vector<int>& p);

		std::vector<int> GetPermutation(int pSize) const;

		bool NextPermutation(int pSize);

		static IdType PermutationsCount(int pSize);
	};
}
