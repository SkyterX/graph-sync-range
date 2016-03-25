#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/algo/GraphEnumeration.h>
#include <graph/algo/StrongConnectivity.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std;
using namespace std::experimental;

namespace graph_algo_tests
{
	TEST_CLASS(StrongConnectivity_Tests) {
	public:

		static const int TestGraphSize = 5;
		static const int TestGraphDegree = 2;

		TEST_METHOD(SimpleChecker_Test) {
			SimpleGraphEnumerator graphEnumerator(TestGraphSize, 2);
			StrongConnectivityChecker_Simple SimpleChecker;
			StrongConnectivityChecker_KosarajuSharir KosarajuSharirChecker;

			do {
				bool ksResult = KosarajuSharirChecker.IsStronglyConnected(graphEnumerator.Current);
				bool simpleResult = SimpleChecker.IsStronglyConnected(graphEnumerator.Current);
				Assert::AreEqual(
					ksResult,
					simpleResult);
			}
			while (graphEnumerator.MoveNext());
		}

	};
}
