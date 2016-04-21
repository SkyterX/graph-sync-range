#include <CppUnitTest.h>
#include <util/Permutations.h>
#include <test_tools/all-test-tools.h>
#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace util;
using namespace std;

namespace util_tests
{
	TEST_CLASS(Permutations_Tests) {
	public:

		TEST_METHOD(PermutationsCount_Test) {
			for (int permutationSize = 1; permutationSize <= Permutation::MaxElementsCount; ++permutationSize) {
				Assert::AreEqual(
					Factorial::Of(permutationSize),
					LazyPermutation::PermutationsCount(permutationSize),
					L"Wrong number of permutations, should be n!");
			}
		}

		TEST_METHOD(PermutationEnumeration_Test) {
			for (int permutationSize = 1; permutationSize <= Permutation::MaxElementsCount; ++permutationSize) {

				// Generate initial permutation
				vector<int> permutation(permutationSize);
				iota(permutation.begin(), permutation.end(), 0);
				auto lazyPermutation = LazyPermutation(0);

				LazyPermutation::IdType currentId = 0;
				bool hasNext = false;
				do {
					Assert::AreEqual(currentId, lazyPermutation.permutationId,
					                 L"Next permutation : wrong id");
					auto lp = lazyPermutation.GetPermutation(permutationSize);
					Assert::AreEqual(lp, permutation,
					                 L"Wrong permutation by id");
					Assert::AreEqual(currentId, LazyPermutation(permutation).permutationId,
					                 L"Wrong id by permutation");
					Assert::AreEqual(permutation, LazyPermutation(currentId).GetPermutation(permutationSize),
					                 L"Wrong permutation by just id");
					hasNext = next_permutation(permutation.begin(), permutation.end());
					Assert::AreEqual(hasNext, lazyPermutation.NextPermutation(permutationSize),
					                 L"Next permutation : wrong end condition");
					++currentId;
				}
				while (hasNext);
			}
		}
	};
}
