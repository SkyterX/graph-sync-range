#pragma once
#include <queue>
#include <vector>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <graph/graph-stats.h>

namespace graph
{
	namespace stats
	{
		extern TimeMeasure SynchronizationCheckTime;
	}

	class SynchronizationChecker {
		std::queue<int> q;
		std::vector<bool> used;
		int n, k;
		int targetNodesCount;
		Graph automata;

		Graph pg;

	public:

		explicit SynchronizationChecker(int nVertices, int outDegree);

		bool IsSynchronizing(const Graph& graph, const GraphColoring& coloring);

	private:
		void Clear();

		void BuildAutomata(const Graph& graph, const GraphColoring& coloring);

		void BuildPGraph();

		bool CheckReachability();
	};
}
