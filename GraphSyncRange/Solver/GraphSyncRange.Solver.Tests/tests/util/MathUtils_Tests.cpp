#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace util;
using namespace std;

namespace util_tests
{
	TEST_CLASS(FastPower_Test) {
	public:

		TEST_METHOD(FastPower_Tests) {
			Simple_Tests<short>();
			Simple_Tests<int>();

			Simple_Tests<float>();
			Simple_Tests<double>();
			TestPower<double>(0.5, 10);
		}

		template <typename TVal>
		void Simple_Tests() {
			vector<TVal> valuesToTest = {
				static_cast<TVal>(0),
				static_cast<TVal>(1),
				static_cast<TVal>(-1),
				static_cast<TVal>(2),
				static_cast<TVal>(3),
				static_cast<TVal>(5)};
			vector<int> powersToTest = {0, 1, 2, 3, 5};
			for (auto& val : valuesToTest) {
				for (auto& p : powersToTest) {
					TestPower(val, p);
				}
			}
		}

		template <typename TVal>
		void TestPower(TVal val, int p) {
			auto expected = pow(static_cast<double>(val), static_cast<double>(p));
			auto actual = static_cast<double>(FastPower(val, p));

			Assert::AreEqual(expected, actual);
		}
	};
}
