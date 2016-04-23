#include <vector>
#include <cassert>
#include <graph/algo/LazySyncRange.h>
#include <graph/algo/SyncRange.h>
#include <graph/algo/Synchronization.h>
#include <graph/GraphIO.h>
#include <graph/algo/StrongConnectivity.h>
#include <graph/algo/Aperiodicity.h>
#include <graph/enumeration/ColoringsGraph.h>

using namespace std;
using namespace graph::stats;
using namespace util;

namespace graph
{
	LazySyncRangeChecker::LazySyncRangeChecker(int verticesCount, int outDegree, int minRangeToLog)
		: n(verticesCount), k(outDegree),
		  minRangeToLog(minRangeToLog),
		  totalColoringsCount(GraphColoring::ColoringsCount(verticesCount, outDegree)),
		  coloringsEnumerator(verticesCount, outDegree),
		  coloringsGraph(BuildColoringsGraph(verticesCount, outDegree)) {
		q.Resize(totalColoringsCount);
		distance.resize(totalColoringsCount);
		parent.resize(totalColoringsCount);
		reachableColorings.resize(totalColoringsCount);
	}

	int LazySyncRangeChecker::CheckSyncRange(const Graph& graph) {
		assert(StrongConnectivityChecker_Simple().IsStronglyConnected(graph));
		assert(AperiodicityChecker().IsAperiodic(graph));

		Clear();

		if (!ProcessSyncColorings(graph)) {
			return 0;
		}

		auto farthestColoringId = FindFarthestUnsyncColoring();

		LogStats(graph, farthestColoringId);

		return distance[farthestColoringId];
	}

	void LazySyncRangeChecker::Clear() {
		fill(distance.begin(), distance.end(), -1);
		fill(parent.begin(), parent.end(), 0);
		fill(reachableColorings.begin(), reachableColorings.end(), false);

		q.Clear();
	}

	bool LazySyncRangeChecker::ProcessSyncColorings(const Graph& graph) {

		coloringsEnumerator.EnumerateColoringsOf(graph);

		auto syncColoringsCount = 0;
		auto reachableColoringsCount = 0;
		while (coloringsEnumerator.MoveNext()) {
			auto& coloringId = coloringsEnumerator.Current;
			q.Push(coloringId);
			distance[coloringId] = 0;
			parent[coloringId] = coloringId;
			++syncColoringsCount;

			if (!reachableColorings[coloringId]) {
				++reachableColoringsCount;
				reachableColorings[coloringId] = true;
			}
			for (const auto& neighborId : coloringsGraph.edges[coloringId]) {
				if (reachableColorings[neighborId]) continue;
				++reachableColoringsCount;
				reachableColorings[neighborId] = true;
			}
			if (reachableColoringsCount == totalColoringsCount) {
				return false;
			}
		}

		double syncRatio = (0.0 + syncColoringsCount) / totalColoringsCount;
		if (syncRatio < minSyncRatio) {
			minGraph = graph;
			minSyncRatio = syncRatio;
		}
		sumSyncRatio += syncRatio;
		sumSqrSyncRatio += syncRatio * syncRatio;
		++cntGraphs;

		assert(syncColoringsCount > 0);
		return 0 < syncColoringsCount && syncColoringsCount < totalColoringsCount;
	}

	LazySyncRangeChecker::ColoringIdType LazySyncRangeChecker::FindFarthestUnsyncColoring() {
		CreateTimestamp();
		ColoringIdType farthestColoringId = -1;
		auto coloringsProcessed = q.Size();
		while (coloringsProcessed < totalColoringsCount && !q.IsEmpty()) {
			auto coloringId = q.Pop();

			for (const auto& toId : coloringsGraph.edges[coloringId]) {
				// bfs step
				if (distance[toId] != -1) continue;
				distance[toId] = distance[coloringId] + 1;
				parent[toId] = coloringId;
				q.Push(toId);
				++coloringsProcessed;

				farthestColoringId = toId;
			}
		}
		UpdateTimer(SyncRangeComputationTime);

		assert(coloringsProcessed == totalColoringsCount); // neighbors generation failed
		return farthestColoringId;
	}

	int maxRangesFound = 0;

	void LazySyncRangeChecker::LogStats(const Graph& graph, ColoringIdType farthestColoringId) {

		if (minRangeToLog > 0 && distance[farthestColoringId] >= minRangeToLog) {
			printf("%d : ", ++maxRangesFound);
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
