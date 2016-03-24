#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>

namespace graph
{
	void FindSyncRange(const Graph& graph);
	std::vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph);

//	Graph minGraph(0);
//	double minSyncRatio = 1.0;
//	double sumSyncRatio = 0.0;
//	double sumSqrSyncRatio = 0.0;
//	double cntGraphs = 0;
}