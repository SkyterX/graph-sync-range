#include <graph/algo/StronglyConnectedComponents.h>

using namespace std;

namespace graph
{
	vector<int> StronglyConnectedComponentsBuilder_KosarajuSharir::FindComponents(const Graph& graph) {
		this->graph = &graph;

		Clean();
		BuildReverseGraph();
		BuildTopologicalOrder();
		BuildComponents();
		return componentIds;
	}

	void StronglyConnectedComponentsBuilder_KosarajuSharir::Clean() {
		auto n = graph->VerticesCount();
		reversedGraph.edges.resize(n);
		for (int v = 0; v < n; ++v) {
			reversedGraph.edges[v].clear();
		}

		topologicalOrder.clear();
		componentIds.assign(n, 0);
	}

	void StronglyConnectedComponentsBuilder_KosarajuSharir::BuildReverseGraph() {
		for (int v = 0; v < graph->VerticesCount(); ++v) {
			for (auto& to : graph->edges[v]) {
				reversedGraph.AddEdge(to, v);
			}
		}
	}

	void StronglyConnectedComponentsBuilder_KosarajuSharir::BuildTopologicalOrder() {
		auto n = graph->VerticesCount();
		visited.assign(n, false);
		for (int v = 0; v < n; ++v) {
			if (!visited[v])
				BuildTopologicalOrderRecursive(v);
		}
		reverse(topologicalOrder.begin(), topologicalOrder.end());
	}

	void StronglyConnectedComponentsBuilder_KosarajuSharir::BuildTopologicalOrderRecursive(int v) {
		visited[v] = true;
		for (auto& to : graph->edges[v]) {
			if (!visited[to])
				BuildTopologicalOrderRecursive(to);
		}
		topologicalOrder.push_back(v);
	}

	void StronglyConnectedComponentsBuilder_KosarajuSharir::BuildComponents() {
		auto n = graph->VerticesCount();
		visited.assign(n, false);
		int nextComponentId = 0;
		for (auto& v : topologicalOrder) {
			if (!visited[v])
				BuildComponentsRecursive(v, nextComponentId++);
		}
	}

	void StronglyConnectedComponentsBuilder_KosarajuSharir::BuildComponentsRecursive(int v, int componentId) {
		visited[v] = true;
		componentIds[v] = componentId;
		for (auto& to : reversedGraph.edges[v]) {
			if (!visited[to])
				BuildComponentsRecursive(to, componentId);
		}
	}
}
