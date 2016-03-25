#pragma once
#include <graph/Graph.h>
#include <vector>

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

	template<typename SCCBuilder>
	struct StrongConnectivityChecker {
		bool IsStronglyConnected(const Graph& graph);
	private:
		SCCBuilder builder;
	};

	struct StronglyConnectedComponentsBuilder_KosarajuSharir {

		std::vector<int> FindComponents(const Graph& graph);

	private:
		const Graph* graph;
		Graph reversedGraph;
		std::vector<int> topologicalOrder;
		std::vector<int> componentIds;
		std::vector<bool> visited;

		void Clean();
		void BuildReverseGraph();
		void BuildTopologicalOrder();
		void BuildTopologicalOrderRecursive(int v);
		void BuildComponents();
		void BuildComponentsRecursive(int v, int componentId);
	};

	template<>
	StrongConnectivityChecker<StronglyConnectedComponentsBuilder_KosarajuSharir>;

}
