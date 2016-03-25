#include <graph/algo/StrongConnectivity.h>
#include <graph/Graph.h>
#include <algorithm>

using namespace std;

namespace graph
{
	bool StrongConnectivityChecker_Simple::IsStronglyConnected(const Graph& graph) {
		int n = graph.VerticesCount();
		this->graph = &graph;
		depth.assign(n, -1);
		fup.assign(n, -1);

		dfs(0);
		for (int v = 0; v < n; ++v) {
			if (fup[v] != 0)
				return false;
		}
		return true;
	}

	void StrongConnectivityChecker_Simple::dfs(int v, int curDepth) {
		depth[v] = curDepth;
		fup[v] = curDepth;
		for (auto& to : graph->edges[v]) {
			if (depth[to] != -1)
				fup[v] = min(fup[v], depth[to]);
			else {
				dfs(to, curDepth + 1);
				fup[v] = min(fup[v], fup[to]);
			}
		}
	}
}

namespace graph
{
	template <typename SCCBuilder>
	bool StrongConnectivityChecker<SCCBuilder>::IsStronglyConnected(const Graph& graph) {
		vector<int> components = builder.FindComponents(graph);
		return all_of(components.begin(), components.end(),
		              [](int x) {
			              return x == 0;
		              });
	}
}

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
