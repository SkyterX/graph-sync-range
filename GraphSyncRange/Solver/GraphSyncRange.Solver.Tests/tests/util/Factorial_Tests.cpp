#include "CppUnitTest.h"
#include <util/Factorial.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace util;

namespace util_tests
{
	TEST_CLASS(Factorial_Tests) {
	public:

		TEST_METHOD(FactorialValues_Test) {
			Assert::AreEqual(Factorial::Of(0), 1);

			auto factorialsCount = Factorial::All().size();

			Factorial::IdType factorial = 1;
			for (int i = 1; i < factorialsCount; ++i) {
				factorial *= i;
				Assert::AreEqual(Factorial::Of(i), factorial);
			}
		}

	};
}