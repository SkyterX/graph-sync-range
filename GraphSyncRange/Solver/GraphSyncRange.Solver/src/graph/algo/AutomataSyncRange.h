#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>

namespace graph
{
	int FindSyncRange(const Graph& graph, const GraphColoring& coloring);
}
