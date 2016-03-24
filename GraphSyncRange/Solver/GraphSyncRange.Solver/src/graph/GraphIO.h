#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <memory>

namespace graph
{
	std::unique_ptr<Graph> ReadGraph();
	void PrintGraph(const Graph& graph);
	void PrintColoring(const GraphColoring& coloring);
}
