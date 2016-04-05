#pragma once
#include <graph/Graph.h>
#include <graph/enumeration/SyncColoringsEnumeration.h>

namespace graph
{
	void FindSyncRange(const Graph& graph, int minRange = 3);

	extern Graph minGraph;
	extern double minSyncRatio;
	extern double sumSyncRatio;
	extern double sumSqrSyncRatio;
	extern double cntGraphs;
}