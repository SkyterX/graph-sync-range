#pragma once
#include <graph/Graph.h>
#include <graph/enumeration/SyncColoringsEnumeration.h>
#include <graph/graph-stats.h>

namespace graph
{
	namespace stats
	{
		extern Graph minGraph;
		extern double minSyncRatio;
		extern double sumSyncRatio;
		extern double sumSqrSyncRatio;
		extern double cntGraphs;

		extern TimeMeasure SyncRangeComputationTime;
	}

	class SyncRangeChecker {
		using ColoringIdType = GraphColoring::IdType;

		int n, k;

		int totalColoringsCount;
		std::vector<int> distance;
		std::vector<ColoringIdType> parent;
		std::queue<ColoringIdType> q;
		SyncColoringsEnumerator coloringsEnumerator;

	public:

		SyncRangeChecker(int verticesCount, int outDegree);

		int MinRangeToLog;

		int CheckSyncRange(const Graph& graph);

	private:

		void Clear();
		ColoringIdType ProcessSyncColorings(const Graph& graph);
		ColoringIdType FindFarthestUnsyncColoring();
		void LogStats(const Graph& graph, ColoringIdType farthestColoringId);
	};
}
