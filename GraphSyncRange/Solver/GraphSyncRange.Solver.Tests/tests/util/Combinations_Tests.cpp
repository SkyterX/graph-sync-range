#include <CppUnitTest.h>
#include <util/Combinations.h>
#include <test_tools/all-test-tools.h>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace util;
using namespace std;
using namespace test_tools;

namespace util_tests
{
	TEST_CLASS(Combinations_Tests) {
	public:

		TEST_METHOD(Combinations_Test) {
			for (int n = 0; n < Combinations::MaxElementsCount; ++n) {
				size_t totalCount = 0;
				for (int k = 0; k <= n; ++k) {
					auto& combinations = Combinations::Of(n, k);
					auto expectedCount = Combinations::Count(n, k);
					Assert::AreEqual(expectedCount, combinations.size(), L"Wrong count");
					totalCount += expectedCount;

					for (size_t i = 1; i < combinations.size(); ++i) {
						Assert::IsTrue(combinations[i - 1] < combinations[i], L"Wrong order");
					}
				}
				Assert::AreEqual(static_cast<size_t>(pow(2, n)), totalCount, L"Wrong total count");
			}
		}

		TEST_METHOD(CombinationsWithRepetitions_Test) {
			Assert::AreEqual(1_z, CombinationsWithRepetitions::Count(0, 0));
			for(int k = 1; k < CombinationsWithRepetitions::MaxElementsCount; ++k)
				Assert::AreEqual(0_z, CombinationsWithRepetitions::Count(0, k));
			for (int n = 1; n < CombinationsWithRepetitions::MaxElementsCount; ++n) {
				for (int k = 0; n + k < CombinationsWithRepetitions::MaxElementsCount; ++k) {
					auto& combinations = CombinationsWithRepetitions::Of(n, k);
					auto expectedCount = CombinationsWithRepetitions::Count(n, k);
					Assert::AreEqual(expectedCount, combinations.size(), L"Wrong count");

					for (size_t i = 1; i < combinations.size(); ++i) {
						Assert::IsTrue(combinations[i - 1] < combinations[i], L"Wrong order");
					}
				}
			}
		}
	};
}
