#include <CppUnitTest.h>
#include <test_tools/all-test-tools.h>
#include <util/SkippingEnumerator.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace util;

namespace util_tests
{
	TEST_CLASS(SkippingEnumerator_Tests) {
	public:

		struct BaseEnumerator {
			using ValueType = int;

			ValueType MaxN;

			explicit BaseEnumerator(ValueType maxN)
				: MaxN(maxN) {}

			ValueType Current = 0;

			bool MoveNext() {
				if (++Current > MaxN)
					return false;
				return true;
			}
		};

		TEST_METHOD(SkippingEnumerator_Test) {
			for (int maxN = 10; maxN < 20; ++maxN) {
				for (int mod = 1; mod <= maxN; ++mod) {
					for (int remainder = 0; remainder < mod; ++remainder) {
						BaseEnumerator baseEnumerator(maxN);
						auto enumerator = TakeEvery(baseEnumerator, mod, remainder);
						bool hasNext = true;
						for (int x = remainder; x <= maxN; x += mod) {
							Assert::IsTrue(hasNext);
							Assert::AreEqual(x, enumerator.Current);
							hasNext = enumerator.MoveNext();
						}
						Assert::IsFalse(hasNext);
					}
				}
			}
		}

	};
}
