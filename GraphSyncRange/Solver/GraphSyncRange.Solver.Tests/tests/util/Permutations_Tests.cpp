#include "CppUnitTest.h"
#include <util/Permutations.h>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace util;
using namespace std;

namespace util_tests
{
	// TODO Assert descriptions
	// TODO Implement ToString for vector
	TEST_CLASS(Factorial_Tests) {
	public:

		static const int TestPermutationSize = 6;

		TEST_CLASS_INITIALIZE(Permutations_SetUp) {
			Permutation::Generate(TestPermutationSize);
		}

		TEST_METHOD(PermutationsCount_Test) {
			Assert::AreEqual(Factorial::Of(TestPermutationSize), LazyPermutation::PermutationsCount(TestPermutationSize));
		}

		TEST_METHOD(PermutationEnumeration_Test) {
			vector<int> permutation;
			for (int i = 0; i < TestPermutationSize; ++i) {
				permutation.push_back(i);
			}

			auto lazyPermutation = LazyPermutation(0);
			LazyPermutation::IdType currentId = 0;
			bool hasTrueNext = false;
			bool hasLazyNext = false;
			do {
				Assert::AreEqual(currentId, lazyPermutation.permutationId);
				auto lp = lazyPermutation.GetPermutation(TestPermutationSize);
				Assert::IsTrue(lp == permutation);
				Assert::AreEqual(currentId, LazyPermutation(permutation).permutationId);
				Assert::IsTrue(permutation == LazyPermutation(currentId).GetPermutation(TestPermutationSize));
				hasTrueNext = next_permutation(permutation.begin(), permutation.end());
				hasLazyNext = lazyPermutation.NextPermutation(TestPermutationSize);
				Assert::AreEqual(hasTrueNext, hasLazyNext);
				++currentId;
			}
			while (hasTrueNext);
		}
	};
}
