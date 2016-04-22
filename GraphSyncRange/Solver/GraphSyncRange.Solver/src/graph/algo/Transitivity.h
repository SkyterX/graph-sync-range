#pragma once
#include <graph/Graph.h>

namespace graph
{
	Graph BuildTranisitiveGraph(const Graph& graph, int maxTransitiveSteps = -1);
}