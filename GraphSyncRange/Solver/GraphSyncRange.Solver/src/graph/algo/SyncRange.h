#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>

namespace graph
{
	void FindSyncRange(const Graph& graph, int minRange = 3);
	std::vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph);

	extern Graph minGraph;
	extern double minSyncRatio;
	extern double sumSyncRatio;
	extern double sumSqrSyncRatio;
	extern double cntGraphs;
}