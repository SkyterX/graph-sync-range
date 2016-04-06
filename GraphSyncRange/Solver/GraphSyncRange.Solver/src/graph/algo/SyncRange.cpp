#include <vector>
#include <cassert>
#include <graph/algo/SyncRange.h>
#include <graph/algo/Synchronization.h>
#include <graph/GraphIO.h>
#include <graph/algo/StrongConnectivity.h>
#include <graph/algo/Aperiodicity.h>

using namespace std;
using namespace graph::stats;
using namespace util;

namespace graph
{
	namespace stats
	{
		Graph minGraph(0);
		double minSyncRatio = 1.0;
		double sumSyncRatio = 0.0;
		double sumSqrSyncRatio = 0.0;
		double cntGraphs = 0;

		TimeMeasure SyncRangeComputationTime;
	}

	SyncRangeChecker::SyncRangeChecker(int verticesCount, int outDegree)
		: n(verticesCount), k(outDegree),
		  totalColoringsCount(GraphColoring::ColoringsCount(verticesCount, outDegree)),
		  coloringsEnumerator(verticesCount, outDegree),
		  MinRangeToLog(0) {
		q.Resize(totalColoringsCount);
	}

	int SyncRangeChecker::CheckSyncRange(const Graph& graph) {
		assert(StrongConnectivityChecker_Simple().IsStronglyConnected(graph));
		assert(AperiodicityChecker().IsAperiodic(graph));

		Clear();

		ColoringIdType syncColoringsCnt = ProcessSyncColorings(graph);

		if (syncColoringsCnt == totalColoringsCount) {
			//			printf("All colorings are sync\n");
			return 0;
		}

		auto farthestColoringId = FindFarthestUnsyncColoring();

		LogStats(graph, farthestColoringId);

		return distance[farthestColoringId];
	}

	void SyncRangeChecker::Clear() {
		distance.assign(totalColoringsCount, -1);
		parent.assign(totalColoringsCount, 0);

		q.Clear();
	}

	SyncRangeChecker::ColoringIdType SyncRangeChecker::ProcessSyncColorings(const Graph& graph) {

		coloringsEnumerator.EnumerateColoringsOf(graph);

		auto syncCologingsCount = 0;
		while (coloringsEnumerator.MoveNext()) {
			auto& coloringId = coloringsEnumerator.Current;
			q.Push(coloringId);
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

		assert(syncCologingsCount > 0);
		return syncCologingsCount;
	}

	SyncRangeChecker::ColoringIdType SyncRangeChecker::FindFarthestUnsyncColoring() {
		CreateTimestamp();
		ColoringIdType farthestColoringId = -1;
		auto coloringsProcessed = q.Size();
		while (coloringsProcessed < totalColoringsCount && !q.IsEmpty()) {
			auto coloringId = q.Pop();

			auto coloring = GraphColoring(n, k, coloringId);
			// generate neighbors
			for (int v = 0; v < n; ++v) { // select vertex
				auto originalPermutation = coloring.edgeColors[v]; // save original coloring
				auto permutation = originalPermutation.GetPermutation(k);
				for (int a = 0; a < k - 1; ++a) { // select first edge
					for (int b = a + 1; b < k; ++b) { // select second edge
						swap(permutation[a], permutation[b]); // recolor
						coloring.edgeColors[v] = LazyPermutation(permutation);
						swap(permutation[a], permutation[b]); // restore 
						auto toId = coloring.GetId(); // generated neighbor

						// bfs step
						if (distance[toId] != -1) continue;
						distance[toId] = distance[coloringId] + 1;
						parent[toId] = coloringId;
						q.Push(toId);
						++coloringsProcessed;

						farthestColoringId = toId;
					}
				}
				coloring.edgeColors[v] = originalPermutation; // restore original coloring of vertex
			}
		}
		UpdateTimer(SyncRangeComputationTime);

		assert(coloringsProcessed == totalColoringsCount); // neighbors generation failed
		return farthestColoringId;
	}

	int maxRangeId = 0;

	void SyncRangeChecker::LogStats(const Graph& graph, ColoringIdType farthestColoringId) {

		if (MinRangeToLog > 0 && distance[farthestColoringId] >= MinRangeToLog) {
			printf("%d : ", ++maxRangeId);
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
}
