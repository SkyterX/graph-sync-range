#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <util/util-all.h>
#include <graph/graph-all.h>

using namespace std;

using namespace graph::stats;
using namespace graph;
using namespace util;

size_t totalGraphs = 0;
TimeMeasure TotalTime;

void FindMaxSyncRange(int n, int k) {
	StartTimer();

	Permutation::Generate(k);

	auto enumerator = SimpleGraphEnumerator(n, k);
	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;

	totalGraphs = 0;
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

	UpdateTimer(TotalTime);
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

	StartTimer();

	Permutation::Generate(k);
	FindSyncRange(*g);

	UpdateTimer(TotalTime);
}


void FindMaxSyncRangeFile(const char* fileName) {
	StartTimer();

	Graph6Reader reader(fileName);

	reader.MoveNext();
	int k = 0;
	for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
		k = max(k, (int) reader.Current.edges[v].size());
	}

	Permutation::Generate(k);

	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;

	
	totalGraphs = 0;
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
	}
	while (reader.MoveNext());

	UpdateTimer(TotalTime);
}

void PrintStats() {
	using OutputTime = chrono::milliseconds;

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

		printf("\nTime stats\n");
		printf("Total time : %lldms\n", chrono::duration_cast<OutputTime>(TotalTime).count());
		printf("Sync coloring enumeration : %lldms\n", chrono::duration_cast<OutputTime>(SyncColoringsGenerationTime).count());
		printf("Synchronization check : %lldms\n", chrono::duration_cast<OutputTime>(SynchronizationCheckTime).count());
	}
}

int main(void) {
	//	freopen("input.txt", "rt", stdin);
	//	freopen("output.txt", "wt", stdout);

	FindMaxSyncRangeFile(R"(D:\Projects\Study\graphs\directed_7_2_scc.d6)");
	//	FindMaxSyncRange(6, 2);

	PrintStats();

	return 0;
}
