#include <graph/algo/Eulerian.h>
#include <vector>

using namespace std;

namespace graph
{
	bool IsEulerian(const Graph& graph) {
		if (graph.OutDegree() % 2 == 1) return false;
		int n = graph.VerticesCount();
		vector<int> inDegree(n, 0);
		for (int v = 0; v < n; ++v) {
			for (auto& to : graph.edges[v]) {
				++inDegree[to];
			}
		}
		for (int v = 0; v < n; ++v) {
			if (inDegree[v] % 2 == 1)
				return false;
		}
		return true;
	}
}
