#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <util/util-all.h>
#include <graph/graph-all.h>

using namespace std;

using namespace graph;
using namespace util;

void FindMaxSyncRange(int n, int k) {

	Permutation::Generate(k);

	auto enumerator = SimpleGraphEnumerator(n, k);
	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;
	
	size_t totalGraphs = 0;
	do {
		++totalGraphs;
		auto& graph = enumerator.Current;
		if (sccChecker.IsStronglyConnected(graph)) {
			if (aperiodicityChecker.IsAperiodic(graph)) {
				FindSyncRange(graph);
			}
		}
	}
	while (enumerator.MoveNext());

		if (cntGraphs > 0) {
			printf("\nTotal sync grahs : %d of %lu\n", (int)cntGraphs, totalGraphs);
			double mean = sumSyncRatio / cntGraphs;
			double variance = (sumSqrSyncRatio - sumSyncRatio * sumSyncRatio / cntGraphs) / (cntGraphs - 1);
			double stdDeviation = sqrt(variance);
			printf("Sync ratio Mean     : %lf\n", mean);
			printf("Sync ratio Variance : %lf\n", variance);
			printf("Sync ratio Std Dev  : %lf\n", stdDeviation);
			printf("Sync ratio Min      : %lf\n", minSyncRatio);
			PrintGraph(minGraph);
			printf("\n");
		}		
}

void FindMaxSyncRangeInteractive() {
	int n, k;
	scanf("%d%d", &n, &k);
	FindMaxSyncRange(n, k);
}

void FindGraphSyncRange() {

	auto g = ReadGraph();
	int n = g->VerticesCount();
	int k = g->OutDegree();

	Permutation::Generate(k);
	FindSyncRange(*g);
}


void FindMaxSyncRangeFile(const char* fileName) {
	
	Graph6Reader reader(fileName);
	
	reader.MoveNext();
	int k = 0;
	for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
		k = max(k, (int) reader.Current.edges[v].size());
	}

	Permutation::Generate(k);

	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;

	size_t totalGraphs = 0;
	do {
		++totalGraphs;
		auto& graph = reader.Current;
		if (aperiodicityChecker.IsAperiodic(graph)) {
			for (int v = 0; v < graph.VerticesCount(); ++v) {
				while (graph.edges[v].size() < k)
					graph.AddEdge(v, v);
			}
			FindSyncRange(graph, 3);
		}
	} while (reader.MoveNext());

	if (cntGraphs > 0) {
		printf("\nTotal sync grahs : %d of %lu\n", (int)cntGraphs, totalGraphs);
		double mean = sumSyncRatio / cntGraphs;
		double variance = (sumSqrSyncRatio - sumSyncRatio * sumSyncRatio / cntGraphs) / (cntGraphs - 1);
		double stdDeviation = sqrt(variance);
		printf("Sync ratio Mean     : %lf\n", mean);
		printf("Sync ratio Variance : %lf\n", variance);
		printf("Sync ratio Std Dev  : %lf\n", stdDeviation);
		printf("Sync ratio Min      : %lf\n", minSyncRatio);
		PrintGraph(minGraph);
		printf("\n");
	}
}

int main(void) {
//	freopen("input.txt", "rt", stdin);
//	freopen("output.txt", "wt", stdout);

	FindMaxSyncRangeFile(R"(D:\Projects\Diploma\graphs\directed_7_3_scc.d6)");
//	FindMaxSyncRange(6, 2);

	return 0;
}
