#pragma once
#include <vector>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <graph/graph-stats.h>
#include <collections/SimpleQueue.hpp>

namespace graph
{
	namespace stats
	{
		extern TimeMeasure SynchronizationCheckTime;
		extern TimeMeasure SynchronizationCheck_BuildAutomataTime;
		extern TimeMeasure SynchronizationCheck_BuildPGTime;
		extern TimeMeasure SynchronizationCheck_ReachabilityCheckTime;
		extern int SynchronizationChecksCount;
	}


	class SynchronizationChecker {
		collections::SimpleQueue<int> q;
		std::vector<uint8_t> used;
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
