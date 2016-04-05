#pragma once
#include <vector>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>

namespace graph
{
	std::vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph);

}
