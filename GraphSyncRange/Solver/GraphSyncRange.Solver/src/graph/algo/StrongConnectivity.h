#pragma once
#include <graph/Graph.h>
#include <vector>
#include <graph/algo/StronglyConnectedComponents.h>

namespace graph
{
	struct StrongConnectivityChecker_Simple {

		bool IsStronglyConnected(const Graph& graph);

	private:
		const Graph* graph;
		std::vector<int> depth;
		std::vector<int> fup;

		void dfs(int v, int curDepth = 0);
	};

	template <typename SCCBuilder>
	struct StrongConnectivityChecker {

		StrongConnectivityChecker() {}

		bool IsStronglyConnected(const Graph& graph);

	private:
		SCCBuilder builder;
	};

	template struct StrongConnectivityChecker<StronglyConnectedComponentsBuilder_KosarajuSharir>;
	using StrongConnectivityChecker_KosarajuSharir = StrongConnectivityChecker<StronglyConnectedComponentsBuilder_KosarajuSharir>;
}
