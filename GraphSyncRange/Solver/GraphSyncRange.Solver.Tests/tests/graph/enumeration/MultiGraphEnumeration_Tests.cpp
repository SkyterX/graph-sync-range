#include <CppUnitTest.h>
#include <graph/enumeration/MultiGraphEnumeration.hpp>
#include <test_tools/all-test-tools.h>
#include <graph/Graph.h>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace test_tools;
using namespace graph;

namespace util_tests
{
	class GraphList {

		vector<Graph> graphs;
		vector<Graph>::const_iterator current;

	public:

		GraphList(initializer_list<vector<vector<int>>> edgesList) {
			for (auto& edges : edgesList) {
				graphs.push_back(Graph());
				graphs.back().edges = edges;
			}
			current = graphs.begin();
			Current = *current;
		}

		bool MoveNext() {
			++current;
			if (current == graphs.end())
				return false;
			Current = *current;
			return true;
		}

		Graph Current;
	};

	TEST_CLASS(MultiGraphEnumeration_Tests) {
	public:

		TEST_METHOD(MultiGraphEnumeration_Test) {

			GraphList graphs = {
				{{1},{0}},
				{{1},{0, 2},{1, 2, 3},{}},
				{{1, 1, 1},{2, 2, 2},{3, 3, 3},{4, 4, 4},{0, 0, 0}}
			};

			GraphList expectedGraphs = {
				{{1, 0, 0},{0, 1, 1}},
				{{1, 0, 1},{0, 1, 1}},
				{{1, 1, 1},{0, 1, 1}},
				{{1, 0, 0},{0, 1, 0}},
				{{1, 0, 1},{0, 1, 0}},
				{{1, 1, 1},{0, 1, 0}},
				{{1, 0, 0},{0, 0, 0}},
				{{1, 0, 1},{0, 0, 0}},
				{{1, 1, 1},{0, 0, 0}},
				{{1, 0, 0},{0, 2, 1},{1, 2, 3},{3, 3, 3}},
				{{1, 0, 1},{0, 2, 1},{1, 2, 3},{3, 3, 3}},
				{{1, 1, 1},{0, 2, 1},{1, 2, 3},{3, 3, 3}},
				{{1, 0, 0},{0, 2, 0},{1, 2, 3},{3, 3, 3}},
				{{1, 0, 1},{0, 2, 0},{1, 2, 3},{3, 3, 3}},
				{{1, 1, 1},{0, 2, 0},{1, 2, 3},{3, 3, 3}},
				{{1, 0, 0},{0, 2, 2},{1, 2, 3},{3, 3, 3}},
				{{1, 0, 1},{0, 2, 2},{1, 2, 3},{3, 3, 3}},
				{{1, 1, 1},{0, 2, 2},{1, 2, 3},{3, 3, 3}},
				{{1, 1, 1},{2, 2, 2},{3, 3, 3},{4, 4, 4},{0, 0, 0}}
			};

			auto multiGraphs = EnumerateMultiGraphs(3, graphs);

			bool hasNext;
			do {
				Assert::AreEqual(expectedGraphs.Current.edges, multiGraphs.Current.edges);
				hasNext = expectedGraphs.MoveNext();
				Assert::AreEqual(hasNext, multiGraphs.MoveNext());
			}
			while (hasNext);
		}

	};
}
