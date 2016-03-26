#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/algo/GraphEnumeration.h>
#include <graph/algo/StrongConnectivity.h>
#include <test_tools/TimeLapse.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(StrongConnectivity_Tests) {
	public:

		static const int TestGraphSize = 5;
		static const int TestGraphDegree = 2;

		TEST_METHOD(SimpleChecker_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			StrongConnectivityChecker_Simple checker;
			StrongConnectivityChecker_KosarajuSharir validator;

			Timer::DurationType cTime(0), vTime(0);
			SimpleGraphEnumerator graphEnumerator(TestGraphSize, 2);
			do {
				bool expected, actual;
				vTime += Timer::Duration(
					[&validator](const Graph& graph, bool& result) {
						result = validator.IsStronglyConnected(graph);
					}, graphEnumerator.Current, expected);
				cTime += Timer::Duration(
					[&checker](const Graph& graph, bool& result) {
						result = checker.IsStronglyConnected(graph);
					}, graphEnumerator.Current, actual);
				Assert::AreEqual(expected, actual);
			}
			while (graphEnumerator.MoveNext());

			Message::WriteLineF("Checker   : %lld", chrono::duration_cast<ResultTime>(cTime).count());
			Message::WriteLineF("Validator : %lld", chrono::duration_cast<ResultTime>(vTime).count());
		}

	};
}
