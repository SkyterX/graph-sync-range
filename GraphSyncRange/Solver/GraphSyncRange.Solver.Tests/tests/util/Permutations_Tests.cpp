#include <CppUnitTest.h>
#include <util/Permutations.h>
#include <test_tools/ToStringExtensions.hpp>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace util;
using namespace std;

namespace util_tests
{
	TEST_CLASS(Permutations_Tests) {
	public:

		static const int TestPermutationSize = 6;

		TEST_CLASS_INITIALIZE(Permutations_SetUp) {
			for (int permutationSize = 1; permutationSize <= TestPermutationSize; ++permutationSize) {
				Permutation::Generate(permutationSize);
			}
		}

		TEST_METHOD(PermutationsCount_Test) {
			for (int permutationSize = 1; permutationSize <= TestPermutationSize; ++permutationSize) {
				Assert::AreEqual(
					Factorial::Of(TestPermutationSize),
					LazyPermutation::PermutationsCount(TestPermutationSize),
					L"Wrong number of permutations, should be n!");
			}
		}

		TEST_METHOD(PermutationEnumeration_Test) {
			for (int permutationSize = 1; permutationSize <= TestPermutationSize; ++permutationSize) {

				// Generate initial permutation
				vector<int> permutation;
				for (int i = 0; i < TestPermutationSize; ++i) {
					permutation.push_back(i);
				}
				auto lazyPermutation = LazyPermutation(0);

				LazyPermutation::IdType currentId = 0;
				bool hasNext = false;
				do {
					Assert::AreEqual(currentId, lazyPermutation.permutationId,
					                 L"Next permutation : wrong id");
					auto lp = lazyPermutation.GetPermutation(TestPermutationSize);
					Assert::AreEqual(lp, permutation,
					                 L"Wrong permutation by id");
					Assert::AreEqual(currentId, LazyPermutation(permutation).permutationId,
					                 L"Wrong id by permutation");
					Assert::AreEqual(permutation, LazyPermutation(currentId).GetPermutation(TestPermutationSize),
					                 L"Wrong permutation by just id");
					hasNext = next_permutation(permutation.begin(), permutation.end());
					Assert::AreEqual(hasNext, lazyPermutation.NextPermutation(TestPermutationSize),
					                 L"Next permutation : wrong end condition");
					++currentId;
				}
				while (hasNext);
			}
		}
	};
}
