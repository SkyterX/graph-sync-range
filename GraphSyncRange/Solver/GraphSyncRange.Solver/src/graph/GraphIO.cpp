#define _CRT_SECURE_NO_WARNINGS

#include <graph/GraphIO.h>
#include <util/UtilIO.h>
#include <algorithm>
#include <string>

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

	void PrintDotGraph(const Graph& graph, FILE* file) {
		vector<string> nodeLabels;
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			nodeLabels.push_back(to_string(v));
		}
		PrintDotGraph(graph, nodeLabels, file);
	}

	void PrintDotGraph(const Graph& graph, const vector<string>& nodeLabels, FILE* file) {
		fprintf_s(file, "digraph SomeDigraph {\n");
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			auto& label = nodeLabels[v];
			fprintf_s(file, "\t%d [label=\"%s\"];\n", v, label.c_str(), label.length());
		}
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			char label = 'a';
			for(auto& to : graph.edges[v]) {
				fprintf_s(file, "\t%d -> %d [label=\"%c\"];\n", v, to, label++);
			}
		}
		fprintf_s(file, "}");
	}
}
