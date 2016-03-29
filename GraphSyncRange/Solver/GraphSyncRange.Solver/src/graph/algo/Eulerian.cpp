#include <graph/algo/Eulerian.h>
#include <vector>

using namespace std;

namespace graph
{
	bool IsEulerian(const Graph& graph) {
		int n = graph.VerticesCount();
		vector<int> degreeDiff(n, 0);
		for (int v = 0; v < n; ++v) {
			degreeDiff[v] += graph.edges[v].size();
			for (auto& to : graph.edges[v]) {
				--degreeDiff[to];
			}
		}
		for (int v = 0; v < n; ++v) {
			if (degreeDiff[v] != 0)
				return false;
		}
		return true;
	}
}
