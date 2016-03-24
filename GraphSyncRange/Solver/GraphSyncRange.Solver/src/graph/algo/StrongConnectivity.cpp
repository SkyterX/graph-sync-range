#include <graph/algo/StrongConnectivity.h>
#include <graph/Graph.h>
#include <algorithm>

using namespace std;

namespace graph {
	StrongConnectivityChecker::StrongConnectivityChecker(const Graph& graph): graph(graph) {}

	bool StrongConnectivityChecker::Check() {
		int n = graph.VerticesCount();
		depth.assign(n, -1);
		fup.assign(n, -1);

		dfs(0);
		for (int v = 0; v < n; ++v) {
			if (fup[v] != 0)
				return false;
		}
		return true;
	}

	void StrongConnectivityChecker::dfs(int v, int curDepth) {
		depth[v] = curDepth;
		fup[v] = curDepth;
		for (auto& to : graph.edges[v]) {
			if (depth[to] != -1)
				fup[v] = min(fup[v], depth[to]);
			else {
				dfs(to, curDepth + 1);
				fup[v] = min(fup[v], fup[to]);
			}
		}
	}
}