#pragma once
#include <vector>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <graph/graph-stats.h>

namespace graph
{
	namespace stats
	{
		extern TimeMeasure SyncColoringsGenerationTime;
	}

	std::vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph);
}
