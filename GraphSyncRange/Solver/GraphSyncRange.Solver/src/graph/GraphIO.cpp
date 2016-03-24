#define _CRT_SECURE_NO_WARNINGS

#include <graph/GraphIO.h>
#include <util/UtilIO.h>

using namespace std;
using namespace util;

namespace graph
{
	unique_ptr<Graph> ReadGraph() {
		int n, m;
		scanf("%d%d", &n, &m);
		auto g = make_unique<Graph>(n);
		for (int i = 0; i < m; ++i) {
			int u, v;
			scanf("%d%d", &u, &v);
			g->AddEdge(u - 1, v - 1);
		}

		return g;
	}

	void PrintGraph(const Graph& graph) {
		printf(" {");
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			printf("%d : (", v);
			for (int i = 0; i < graph.edges[v].size(); ++i) {
				if (i > 0) printf(", ");
				printf("%d", graph.edges[v][i]);
			}
			printf("), ");
		}
		printf("} ");
	}


	void PrintColoring(const GraphColoring& coloring) {
		printf(" {%d :", coloring.GetId());
		for (auto& edgeColor : coloring.edgeColors) {
			PrintPermutation(edgeColor, coloring.nEdges);
		}
		printf("} ");
	}
}