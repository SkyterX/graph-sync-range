#pragma once
#include <vector>
#include <graph/Graph.h>

namespace graph
{
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
}
