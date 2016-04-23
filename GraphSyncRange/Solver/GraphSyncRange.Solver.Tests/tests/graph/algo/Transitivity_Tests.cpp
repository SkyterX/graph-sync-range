#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/all-test-tools.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/Transitivity.h>
#include <collections/SimpleQueue.hpp>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(Transitivity_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(Transitivity_Test) {
			Graph6Reader reader(graphsFileName);

			int n = reader.Current.VerticesCount();

			do {
				for(int transitivityDegree = 0; transitivityDegree < n; ++transitivityDegree) {
					CheckTransitiveGraph(reader.Current, BuildTranisitiveGraph(reader.Current, transitivityDegree), transitivityDegree);
				}
			}
			while (reader.MoveNext());
		}

		void CheckTransitiveGraph(const Graph& graph, const Graph& transitiveGraph, int transitivityDegree) const {
			
			Assert::AreEqual(graph.VerticesCount(), transitiveGraph.VerticesCount());
			for(int v = 0; v < graph.VerticesCount(); ++v) {
				auto expectedEdges = GetReachableVertices(graph, v, transitivityDegree);
				auto actualEdges = transitiveGraph.edges[v];
				stable_sort(expectedEdges.begin(), expectedEdges.end());
				stable_sort(actualEdges.begin(), actualEdges.end());
				Assert::AreEqual(expectedEdges, actualEdges);
			}
		}

		vector<int> GetReachableVertices(const Graph& graph, int startVertex, int maxDistance) const {
			vector<int> reachableVertices;
			vector<int> distance(graph.VerticesCount(), -1);
			distance[startVertex] = 0;
			collections::SimpleQueue<int> q(graph.VerticesCount());
			q.Push(startVertex);
			while(!q.IsEmpty()) {
				auto v = q.Pop();
				if (distance[v] > maxDistance)
					break;
				reachableVertices.push_back(v);
				for(const auto& to : graph.edges[v]) {
					if (distance[to] != -1) continue;
					distance[to] = distance[v] + 1;
					q.Push(to);
				}
			}
			return reachableVertices;
		}
	};
}
