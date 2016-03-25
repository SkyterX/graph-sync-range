#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace util;
using namespace std;
using namespace std::experimental;

namespace graph_algo_tests
{
	TEST_CLASS(StrongConnectivity_Tests) {
	public:

		TEST_METHOD(StrongConnectivity_Test) {
			for (auto & x : Range(10))
				Message::WriteLineF("%d", x);
		}

		generator<int> Range(int n) {
			for (int i = 0; i < n; ++i) {
				yield i;
			}
		}
	};
}
