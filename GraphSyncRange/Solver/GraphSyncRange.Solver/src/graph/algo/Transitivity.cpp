#include <graph/algo/Transitivity.h>
#include <vector>
#include <unordered_set>

using namespace std;

namespace graph
{
	Graph BuildTranisitiveGraph(const Graph& graph, int maxTransitionSteps) {
		auto n = graph.VerticesCount();
		if (maxTransitionSteps == -1)
			maxTransitionSteps = n-1;

		vector<unordered_set<int>> transitiveEdges(n);
		for (int v = 0; v < n; ++v) {
			transitiveEdges[v].insert(v);
		}

		for (int step = 0; step < maxTransitionSteps; ++step) {
			for (int v = 0; v < n; ++v) {
				vector<int> currentEdges(transitiveEdges[v].begin(), transitiveEdges[v].end());
				for (auto& from : currentEdges) {
					transitiveEdges[v].insert(graph.edges[from].begin(), graph.edges[from].end());
				}
			}
		}

		Graph transitiveGraph(n);

		for (int v = 0; v < n; ++v) {
			transitiveGraph.edges[v].resize(transitiveEdges[v].size());
			copy(transitiveEdges[v].begin(), transitiveEdges[v].end(), transitiveGraph.edges[v].begin());
		}
		return transitiveGraph;
	}
}
