#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <memory>

namespace graph
{
	std::unique_ptr<Graph> ReadAutomata(FILE* inputFile = stdin);
	std::unique_ptr<Graph> ReadGraph(FILE* inputFile = stdin);
	void PrintGraph(const Graph& graph);
	void PrintColoring(const GraphColoring& coloring);
}
