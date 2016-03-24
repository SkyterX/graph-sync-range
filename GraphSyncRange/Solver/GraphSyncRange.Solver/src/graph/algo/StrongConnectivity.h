#pragma once
#include <graph/Graph.h>
#include <vector>

namespace graph
{
	struct Graph;

	struct StrongConnectivityChecker {

		const Graph& graph;

		explicit StrongConnectivityChecker(const Graph& graph);

		bool Check();

	private:
		std::vector<int> depth;
		std::vector<int> fup;

		void dfs(int v, int curDepth = 0);
	};
}
