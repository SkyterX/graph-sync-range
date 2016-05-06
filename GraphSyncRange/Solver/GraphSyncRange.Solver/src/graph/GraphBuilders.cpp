#include <graph/GraphBuilders.h>
#include <graph/GraphColoring.h>

namespace graph
{
	Graph BuildHypercubeGraph(int dimensions) {
		if (dimensions == 0) {
			Graph g(1);
			g.AddEdge(0, 0);
			return g;
		}
		else {
			auto g1 = BuildHypercubeGraph(dimensions - 1);
			auto n = g1.VerticesCount();
			auto g = Graph(n * 2);
			for (int v = 0; v < n; ++v) {
				for (auto& to : g1.edges[v]) {
					g.AddEdge(v, to);
					g.AddEdge(v + n, to + n);
				}
				g.AddEdge(v, v + n);
				g.AddEdge(v + n, v);
			}
			return g;
		}
	}

	Graph BuildHyperTriangularGraph(int dimensions) {
		if (dimensions == 0) {
			Graph g(2);
			g.AddEdge(0, 1);
			g.AddEdge(1, 0);
			return g;
		}
		else {
			int k = 2 * dimensions + 1;
			auto g1 = BuildHyperTriangularGraph(dimensions - 1);
			auto n = g1.VerticesCount();
			auto g = Graph(n * k);
			for (int v = 0; v < n; ++v) {
				for (auto& to : g1.edges[v]) {
					for (int i = 0; i < k; ++i) {
						g.AddEdge(v + i * n, to + i * n);
					}
				}
			}
			for (int v1 = 0, v2 = n / 2; v1 < n / 2 && v2 < n; ++v1 , ++v2) {
				for (int i = 0; i < k; ++i) {
					g.AddEdge(v1 + i * n, (v1 + (i + 1) * n) % (k * n));
					g.AddEdge((v2 + (i + 1) * n) % (k * n), v2 + i * n);
				}
			}
			return g;
		}
	}

	Graph BuildInverseGraph(const Graph& graph) {
		Graph rG(graph.VerticesCount());
		for(int v = 0; v < graph.VerticesCount();++v) {
			for(auto& to : graph.edges[v]) {
				rG.AddEdge(to, v);
			}
		}
		return rG;
	}
}
