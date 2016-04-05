#include <vector>
#include <cassert>
#include <graph/algo/SyncRange.h>
#include <graph/algo/Synchronization.h>
#include <graph/GraphIO.h>
#include <graph/algo/StrongConnectivity.h>
#include <graph/algo/Aperiodicity.h>

using namespace std;
using namespace util;

namespace graph
{
	Graph minGraph(0);
	double minSyncRatio = 1.0;
	double sumSyncRatio = 0.0;
	double sumSqrSyncRatio = 0.0;
	double cntGraphs = 0;

	void FindSyncRange(const Graph& graph, int minRange) {
		assert(StrongConnectivityChecker_Simple().IsStronglyConnected(graph));
		assert(AperiodicityChecker().IsAperiodic(graph));

		//		PrintGraph(graph);
		//		printf(" -> ");
		using ColoringIdType = GraphColoring::IdType;

		int n = graph.VerticesCount();
		int k = graph.OutDegree();

		auto totalColoringsCount = GraphColoring::ColoringsCount(n, k);

		vector<int> distance(totalColoringsCount, -1);
		vector<ColoringIdType> parent(totalColoringsCount);
		queue<ColoringIdType> q;
		SyncColoringsEnumerator coloringsEnumerator(graph);

		auto syncCologingsCount = 0;
		while (coloringsEnumerator.MoveNext()) {
			auto& coloringId = coloringsEnumerator.Current;
			q.push(coloringId);
			distance[coloringId] = 0;
			parent[coloringId] = coloringId;
			++syncCologingsCount;
		}

		double syncRatio = (0.0 + syncCologingsCount) / totalColoringsCount;
		if (syncRatio < minSyncRatio) {
			minGraph = graph;
			minSyncRatio = syncRatio;
		}
		sumSyncRatio += syncRatio;
		sumSqrSyncRatio += syncRatio * syncRatio;
		++cntGraphs;

		if (syncCologingsCount == totalColoringsCount) {
			//			printf("All colorings are sync\n");
			return;
		}

		ColoringIdType coloringsProcessed = syncCologingsCount;
		int farthestColoringId = -1;
		while (coloringsProcessed < totalColoringsCount && !q.empty()) {
			auto coloringId = q.front();
			q.pop();

			auto coloring = GraphColoring(n, k, coloringId);
			// generate neighbors
			for (int v = 0; v < n; ++v) { // select vertex
				auto originalPermutation = coloring.edgeColors[v]; // save original coloring
				auto permutation = originalPermutation.GetPermutation(k);
				for (int a = 0; a < k - 1; ++a) { // select first edge
					for (int b = a + 1; b < k; ++b) { // select second edge
						swap(permutation[a], permutation[b]); // recolor
						coloring.edgeColors[v] = util::LazyPermutation(permutation);
						swap(permutation[a], permutation[b]); // restore 
						auto toId = coloring.GetId(); // generated neighbor

						// bfs step
						if (distance[toId] != -1) continue;
						distance[toId] = distance[coloringId] + 1;
						parent[toId] = coloringId;
						q.push(toId);
						++coloringsProcessed;

						farthestColoringId = toId;
					}
				}
				coloring.edgeColors[v] = originalPermutation; // restore original coloring of vertex
			}
		}

		assert(farthestColoringId != -1); // neighbors generation failed

		if (distance[farthestColoringId] < minRange) return;
		PrintGraph(graph);
		printf(" -> ");
		printf("MaxDistance : %d from ", distance[farthestColoringId]);
		PrintColoring(GraphColoring(n, k, farthestColoringId));
		auto coloringId = farthestColoringId;
		while (parent[coloringId] != coloringId) {
			coloringId = parent[coloringId];
		}
		printf(" to ");
		PrintColoring(GraphColoring(n, k, coloringId));
		printf("\n");
	}
}
