#pragma once
#pragma once
#include <graph/Graph.h>
#include <graph/enumeration/SyncColoringsEnumeration.h>
#include <collections/SimpleQueue.hpp>

namespace graph
{
	class LazySyncRangeChecker {
		using ColoringIdType = GraphColoring::IdType;

		int n, k;

		int totalColoringsCount;
		std::vector<int> distance;
		std::vector<ColoringIdType> parent;
		std::vector<uint8_t> reachableColorings;
		collections::SimpleQueue<ColoringIdType> q;
		SyncColoringsEnumerator coloringsEnumerator;
		Graph coloringsGraph;

	public:

		LazySyncRangeChecker(int verticesCount, int outDegree);

		int MinRangeToLog;

		int CheckSyncRange(const Graph& graph);

	private:

		void Clear();
		bool ProcessSyncColorings(const Graph& graph);
		ColoringIdType FindFarthestUnsyncColoring();
		void LogStats(const Graph& graph, ColoringIdType farthestColoringId);
	};
}
