#include <graph/algo/StrongConnectivity.h>
#include <graph/Graph.h>
#include <algorithm>

using namespace std;

namespace graph
{
	bool StrongConnectivityChecker_Simple::IsStronglyConnected(const Graph& graph) {
		int n = graph.VerticesCount();
		this->graph = &graph;
		depth.assign(n, -1);
		fup.assign(n, -1);

		dfs(0);
		for (int v = 0; v < n; ++v) {
			if (fup[v] != 0)
				return false;
		}
		return true;
	}

	void StrongConnectivityChecker_Simple::dfs(int v, int curDepth) {
		depth[v] = curDepth;
		fup[v] = curDepth;
		for (auto& to : graph->edges[v]) {
			if (depth[to] != -1)
				fup[v] = min(fup[v], depth[to]);
			else {
				dfs(to, curDepth + 1);
				fup[v] = min(fup[v], fup[to]);
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
