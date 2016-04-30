#define _CRT_SECURE_NO_WARNINGS

#include <graph/GraphIO.h>
#include <util/UtilIO.h>
#include <algorithm>

using namespace std;
using namespace util;

namespace graph
{
	unique_ptr<Graph> ReadAutomata(FILE* inputFile) {
		int n, m;
		fscanf_s(inputFile, "%d%d", &n, &m);
		auto g = make_unique<Graph>(n);
		for (int i = 0; i < m; ++i) {
			int u, v;
			fscanf_s(inputFile, "%d%d", &u, &v);
			g->AddEdge(u - 1, v - 1);
		}
		return g;
	}

	unique_ptr<Graph> ReadGraph(FILE* inputFile) {
		auto g = ReadAutomata(inputFile);

		for (int v = 0; v < g->VerticesCount(); ++v) {
			sort(g->edges[v].begin(), g->edges[v].end());
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
