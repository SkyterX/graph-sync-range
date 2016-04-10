#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/GraphColoring.h>
#include <graph/enumeration/GraphColoringNeighborsEnumeration.h>
#include <test_tools/TimeLapse.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(GraphColoringNeighbors_Tests) {
	public:

		static constexpr int TestGraphSize = 4;
		static constexpr int TestGraphDegree = 3;

		TEST_METHOD(GraphColoringNeighbors_Enumeration_Test) {
			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			Permutation::Generate(TestGraphDegree);

			Timer::DurationType slowTime(0), fastTime(0);

			GraphColoring coloring(TestGraphSize, TestGraphDegree);
			do {
				vector<GraphColoring::IdType> expected;
				slowTime += Timer::Duration([&expected, &coloring] () {
					GraphColoringNeighborsEnumerator_BruteForce slowEnumerator(coloring);
					do {
						expected.push_back(slowEnumerator.Current.GetId());
					}
					while (slowEnumerator.MoveNext());
					stable_sort(expected.begin(), expected.end());
				});


				vector<GraphColoring::IdType> actual;
				fastTime += Timer::Duration([&actual, &coloring]() {
					GraphColoringNeighborsEnumerator fastEnumerator(coloring);
					do {
						actual.push_back(fastEnumerator.Current.GetId());
					}
					while (fastEnumerator.MoveNext());
					stable_sort(actual.begin(), actual.end());
				});

				Assert::AreEqual(expected, actual);
			}
			while (coloring.NextColoring());

			Message::WriteLineF("Graph coloring neighbors enumeration - brute force\t: %lld", chrono::duration_cast<ResultTime>(slowTime).count());
			Message::WriteLineF("Graph coloring neighbors enumeration - fast \t\t: %lld", chrono::duration_cast<ResultTime>(fastTime).count());
		}
	};
}
