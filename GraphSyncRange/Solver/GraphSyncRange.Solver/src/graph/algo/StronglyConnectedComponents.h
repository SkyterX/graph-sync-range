#pragma once
#include <vector>
#include <graph/Graph.h>
#include <stack>

namespace graph
{
	struct StronglyConnectedComponentsBuilder_KosarajuSharir {

		StronglyConnectedComponentsBuilder_KosarajuSharir();

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

	struct StronglyConnectedComponentsBuilder_Tarjan {

		std::vector<int> FindComponents(const Graph& graph);

	private:
		const Graph* graph;
		Graph reversedGraph;
		std::vector<int> lowLink;
		std::vector<int> index;
		std::vector<int> componentIds;
		std::vector<bool> onStack;
		std::stack<int> vertexStack;
		int currentIndex, nextComponentId;

		void Clean();
		void BuildComponents();
		void BuildComponentsRecursive(int v);
	};
}
