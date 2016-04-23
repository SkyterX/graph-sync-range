#include <vector>
#include <cassert>
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
	namespace stats
	{
		Graph minGraph(0);
		double minSyncRatio = 1.0;
		double sumSyncRatio = 0.0;
		double sumSqrSyncRatio = 0.0;
		double cntGraphs = 0;

		TimeMeasure SyncRangeComputationTime;
	}
}

namespace graph
{
	SyncRangeChecker::SyncRangeChecker(int verticesCount, int outDegree, int minRangeToLog)
		: n(verticesCount),k(outDegree),
		  minRangeToLog(minRangeToLog),
		  totalColoringsCount(GraphColoring::ColoringsCount(verticesCount, outDegree)),
		  coloringsEnumerator(verticesCount, outDegree),
		  coloringsGraph(BuildColoringsGraph(verticesCount, outDegree)) {
		q.Resize(totalColoringsCount);
		distance.resize(totalColoringsCount);
		parent.resize(totalColoringsCount);
	}

	int SyncRangeChecker::CheckSyncRange(const Graph& graph) {
		assert(StrongConnectivityChecker_Simple().IsStronglyConnected(graph));
		assert(AperiodicityChecker().IsAperiodic(graph));

		Clear();

		ColoringIdType syncColoringsCnt = ProcessSyncColorings(graph);

		if (syncColoringsCnt == 0 || syncColoringsCnt == totalColoringsCount) {
			//			printf("All colorings are sync\n");
			return 0;
		}

		auto farthestColoringId = FindFarthestUnsyncColoring();

		LogStats(graph, farthestColoringId);

		return distance[farthestColoringId];
	}

	void SyncRangeChecker::Clear() {
		fill(distance.begin(), distance.end(), -1);
		fill(parent.begin(), parent.end(), 0);

		q.Clear();
	}

	SyncRangeChecker::ColoringIdType SyncRangeChecker::ProcessSyncColorings(const Graph& graph) {

		coloringsEnumerator.EnumerateColoringsOf(graph);

		auto syncColoringsCount = 0;
		while (coloringsEnumerator.MoveNext()) {
			auto& coloringId = coloringsEnumerator.Current;
			q.Push(coloringId);
			distance[coloringId] = 0;
			parent[coloringId] = coloringId;
			++syncColoringsCount;
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
		return syncColoringsCount;
	}

	SyncRangeChecker::ColoringIdType SyncRangeChecker::FindFarthestUnsyncColoring() {
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

	int maxRangeId = 0;

	void SyncRangeChecker::LogStats(const Graph& graph, ColoringIdType farthestColoringId) {

		if (minRangeToLog > 0 && distance[farthestColoringId] >= minRangeToLog) {
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
