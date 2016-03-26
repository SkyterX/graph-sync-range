#include <graph/algo/StrongConnectivity.h>
#include <graph/Graph.h>
#include <algorithm>

using namespace std;

namespace graph
{
	bool StrongConnectivityChecker_Simple::IsStronglyConnected(const Graph& graph) {
		int n = graph.VerticesCount();
		this->graph = &graph;
		tin.assign(n, -1);
		lowLink.assign(n, -1);
		timer = 0;

		dfs(0);
		for (int v = 0; v < n; ++v) {
			if (lowLink[v] != 0 && lowLink[v] == tin[v])
				return false;
		}
		return true;
	}

	void StrongConnectivityChecker_Simple::dfs(int v, int curDepth) {
		tin[v] = timer;
		lowLink[v] = timer;
		++timer;
		for (auto& to : graph->edges[v]) {
			if (tin[to] != -1)
				lowLink[v] = min(lowLink[v], tin[to]);
			else {
				dfs(to, curDepth + 1);
				lowLink[v] = min(lowLink[v], lowLink[to]);
			}
		}
	}

	template <typename SCCBuilder>
	bool StrongConnectivityChecker<SCCBuilder>::IsStronglyConnected(const Graph& graph) {
		vector<int> components = builder.FindComponents(graph);
		return std::all_of(components.begin(), components.end(),
						   [](int x) {
							   return x == 0;
						   });
	}
}
