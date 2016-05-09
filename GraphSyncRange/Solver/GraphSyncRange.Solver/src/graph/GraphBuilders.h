#pragma once

#include <graph/Graph.h>

namespace graph
{
	Graph BuildHypercubeGraph(int dimensions);

	Graph BuildHyperTriangularGraph(int dimensions);

	Graph BuildHyperTriangularGraph2(int dimensions);

	Graph BuildInverseGraph(const Graph& graph);

	Graph BuildTuplesGraph(const Graph& graph);
}
