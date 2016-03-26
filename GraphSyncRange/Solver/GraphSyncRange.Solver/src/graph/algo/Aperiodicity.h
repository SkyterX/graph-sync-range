#pragma once
#include <vector>
#include <graph/Graph.h>

namespace graph
{
	struct AperiodicityChecker {

		bool IsAperiodic(const Graph& graph);

	private:
		const Graph* graph;
		std::vector<int> depth;

		void Clear();

		void BuildDepth(int v, int currentDepth);
	};
}