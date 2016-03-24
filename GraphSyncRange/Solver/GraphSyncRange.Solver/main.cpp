#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <util/util-all.h>
#include <graph/graph-all.h>

using namespace std;

using namespace graph;
using namespace util;

void FindMaxSyncRange(int n, int k) {

	Permutation::Generate(k);

	auto enumerator = SimpleGraphEnumerator(n, k);
	do {
		auto& graph = enumerator.Current;
		if (StrongConnectivityChecker(graph).Check()) {
			FindSyncRange(graph);
		}
	}
	while (enumerator.MoveNext());

		if (cntGraphs > 0) {
			printf("\nTotal sync grahs : %d\n", (int)cntGraphs);
			double mean = sumSyncRatio / cntGraphs;
			double variance = (sumSqrSyncRatio - sumSyncRatio * sumSyncRatio / cntGraphs) / (cntGraphs - 1);
			double stdDeviation = sqrt(variance);
			printf("Sync ratio Mean     : %lf\n", mean);
			printf("Sync ratio Variance : %lf\n", variance);
			printf("Sync ratio Std Dev  : %lf\n", stdDeviation);
			printf("Sync ratio Min      : %lf\n", minSyncRatio);
			PrintGraph(minGraph);
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

int main(void) {
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	FindGraphSyncRange();

	return 0;
}
