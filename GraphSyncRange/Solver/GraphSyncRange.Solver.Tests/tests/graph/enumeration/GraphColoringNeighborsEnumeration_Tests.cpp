#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/GraphColoring.h>
#include <graph/enumeration/GraphColoringNeighborsEnumeration.h>
#include <graph/enumeration/ColoringsGraph.h>
#include <test_tools/TimeLapse.hpp>
#include <algorithm>

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

			Message::WriteLineF("Graph coloring neighbors enumeration - brute force\t: %lldms", chrono::duration_cast<ResultTime>(slowTime).count());
			Message::WriteLineF("Graph coloring neighbors enumeration - fast \t\t: %lldms", chrono::duration_cast<ResultTime>(fastTime).count());
		}

		TEST_METHOD(GraphColoringNeighbors_ColoringGraph_Test) {
			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::microseconds;

			const int graphSize = TestGraphSize + 1;

			Permutation::GeneratePermutations(TestGraphDegree);

			Timer::DurationType graphTime(0), enumeratorTime(0);

			Graph coloringsGraph;
			auto buildGraphTime = Timer::Duration([&coloringsGraph, &graphSize] {
				coloringsGraph = BuildColoringsGraph(graphSize, TestGraphDegree);
			});

			GraphColoring coloring(graphSize, TestGraphDegree);
			do {
				vector<GraphColoring::IdType> expected;
				graphTime += Timer::Duration([&expected, &coloring, &coloringsGraph]() {
					expected = coloringsGraph.edges[coloring.GetId()];
					stable_sort(expected.begin(), expected.end());
				});


				vector<GraphColoring::IdType> actual;
				enumeratorTime += Timer::Duration([&actual, &coloring]() {
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

			Message::WriteLineF("Build coloring graph time \t: %lldus", chrono::duration_cast<ResultTime>(buildGraphTime).count());
			Message::WriteLineF("Graph coloring neighbors from graph \t: %lldus", chrono::duration_cast<ResultTime>(graphTime).count());
			Message::WriteLineF("Graph coloring neighbors enumeration  \t: %lldus", chrono::duration_cast<ResultTime>(enumeratorTime).count());
		}
	};
}
