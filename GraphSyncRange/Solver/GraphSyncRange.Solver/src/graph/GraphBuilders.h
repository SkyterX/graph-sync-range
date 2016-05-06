#pragma once

#include <graph/Graph.h>

namespace graph
{
	Graph BuildHypercubeGraph(int dimensions);

	Graph BuildHyperTriangularGraph(int dimensions);

	Graph BuildInverseGraph(const Graph& graph);
}
