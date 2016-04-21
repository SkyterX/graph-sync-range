#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/MultiGraphColoring.h>
#include <util/Combinations.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(GraphColoring_Tests) {
	public:

		static constexpr int TestGraphSize = 4;
		static constexpr int TestGraphDegree = 3;

		TEST_METHOD(MultiGraphColoring_Enumeration_Test) {

			Graph graph;
			graph.edges = {
				{},
				{0},
				{0, 1},
			};

			vector<vector<vector<int>>> expectedEdges = {
				{{0, 0, 0, 0}},
				{{0, 1, 1, 1},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
				{{0, 1, 2, 2},{0, 1, 2, 0},{0, 1, 2, 1},{0, 1, 0, 0},{0, 1, 0, 1},{0, 1, 1, 1}}
			};
			size_t totalColorings = expectedEdges[0].size() * expectedEdges[1].size() * expectedEdges[2].size();
			AssertColoringCount(totalColorings, graph, 4);

			MultiGraphColoring coloring(4);
			coloring.Reset(graph);
			bool hasNext = true;
			for (auto& edges2 : expectedEdges[2]) {
				for (auto& edges1 : expectedEdges[1]) {
					for (auto& edges0 : expectedEdges[0]) {
						Assert::IsTrue(hasNext);
						coloring.Apply(graph);

						Assert::AreEqual(edges0, graph.edges[0]);
						Assert::AreEqual(edges1, graph.edges[1]);
						Assert::AreEqual(edges2, graph.edges[2]);

						hasNext = coloring.MoveNext();
					}
				}
			}

		}

		TEST_METHOD(MultiGraphColoring_Count_Test) {
			Graph graph;
			graph.edges.push_back({});
			AssertColoringCount(1, graph, 0); // {}
			AssertColoringCount(1, graph, 1); // {0}
			AssertColoringCount(1, graph, 2); // {0, 0}
			graph.edges[0].push_back(1);
			AssertColoringCount(1, graph, 1); // {1}
			AssertColoringCount(2, graph, 2); // {1, 0}, {1, 1}
			AssertColoringCount(3, graph, 3); // {1, 0, 0}, {1, 0, 1}, {1, 1, 1}
			graph.edges[0].push_back(2);
			AssertColoringCount(1, graph, 2); // {1, 2}
			AssertColoringCount(3, graph, 3); // {1, 2, 0}, {1, 2, 1}, {1, 2, 2}
			AssertColoringCount(6, graph, 4); // {1, 2, 0, 0}, {1, 2, 0, 1}, {1, 2, 0, 2}, {1, 2, 1, 1}, {1, 2, 1, 2}, {1, 2, 2, 2}
			graph.edges.push_back({0});
			AssertColoringCount(6 * 4, graph, 4);
		}

		void AssertColoringCount(size_t expectedCount, const Graph& graph, size_t outDegree) {
			Assert::AreEqual(expectedCount, MultiGraphColoring::MultiColoringsCount(graph, outDegree));
		}
	};
}
