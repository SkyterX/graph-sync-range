#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <util/util-all.h>
#include <graph/graph-all.h>
#include <chrono>
#include <random>

using namespace std;

using namespace graph::stats;
using namespace graph;
using namespace util;

size_t totalGraphs = 0;
TimeMeasure TotalTime;

void FindMaxSyncRange(int n, int k) {
	CreateTimestamp();

	Permutation::GeneratePermutations(k);

	auto enumerator = SimpleGraphEnumerator(n, k);
	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;
	SyncRangeChecker syncRangeChecker(n, k, 3);

	totalGraphs = 0;
	do {
		++totalGraphs;
		auto& graph = enumerator.Current;
		if (sccChecker.IsStronglyConnected(graph)) {
			if (aperiodicityChecker.IsAperiodic(graph)) {
				syncRangeChecker.CheckSyncRange(graph);
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
	SyncRangeChecker syncRangeChecker(n, k, 3);

	CreateTimestamp();

	Permutation::GeneratePermutations(k);
	syncRangeChecker.CheckSyncRange(*g);

	UpdateTimer(TotalTime);
}


void FindMaxSyncRangeFile(const char* fileName, int minRangeToLog = 2, int logEvery = 100000) {
	CreateTimestamp();

	Graph6Reader reader(fileName);

	int n = reader.Current.VerticesCount();
	int k = reader.Current.MaxOutDegree();

	Permutation::GeneratePermutations(k);

	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;
	LazySyncRangeChecker syncRangeChecker(n, k, minRangeToLog);

	int skipId;
	cin >> skipId;

	auto graphEnumeration = EnumerateMultiGraphs(k, TakeEvery(reader, 4, skipId));

	CreateTimestampVar(cycleStart);
	totalGraphs = 0;
	do {
		++totalGraphs;
		if (totalGraphs % logEvery == 0) {
			auto duration = chrono::duration_cast<chrono::seconds>(GetTimeByVar(cycleStart));
			UpdateTimestampVar(cycleStart);
			fprintf(stderr, "Processed %zu in %llds\n", totalGraphs, duration.count());
		}
		auto& graph = graphEnumeration.Current;
		if (aperiodicityChecker.IsAperiodic(graph)) {
			for (int v = 0; v < graph.VerticesCount(); ++v) {
				while (graph.edges[v].size() < k)
					graph.AddEdge(v, v);
			}
			syncRangeChecker.CheckSyncRange(graph);
		}
	}
	while (graphEnumeration.MoveNext());

	UpdateTimer(TotalTime);
}

void FindMaxSyncRangeRandom(int verticesCount, int outDegree, int minRangeToLog = 2, int logEvery = 100000) {
	CreateTimestamp();

	int n = verticesCount;
	int k = outDegree;

	Permutation::GeneratePermutations(k);

	StrongConnectivityChecker_Simple sccChecker;
	AperiodicityChecker aperiodicityChecker;
	SyncRangeChecker syncRangeChecker(n, k, minRangeToLog);

	RandomGraphGenerator generator(n, k);

	CreateTimestampVar(cycleStart);
	totalGraphs = 0;
	do {
		++totalGraphs;
		if (totalGraphs % logEvery == 0) {
			auto duration = chrono::duration_cast<chrono::seconds>(GetTimeByVar(cycleStart));
			UpdateTimestampVar(cycleStart);
			fprintf(stderr, "Processed %zu in %llds\n", totalGraphs, duration.count());
		}
		auto& graph = generator.Current;
		if (sccChecker.IsStronglyConnected(graph) && aperiodicityChecker.IsAperiodic(graph)) {
			for (int v = 0; v < graph.VerticesCount(); ++v) {
				while (graph.edges[v].size() < k)
					graph.AddEdge(v, v);
			}
			syncRangeChecker.CheckSyncRange(graph);
		}
	}
	while (generator.MoveNext());

	UpdateTimer(TotalTime);
}

void PrintTimeStatsLine(const char* timerDescription, const TimeMeasure& timer, int indentationLevel) {
	using OutputTime = chrono::milliseconds;

	string description(2 * indentationLevel, ' ');
	description += timerDescription;

	printf("%-30s : %lldms\n", description.c_str(), chrono::duration_cast<OutputTime>(timer).count());
}

void PrintStats() {
	using OutputTime = chrono::milliseconds;

	if (cntGraphs > 0) {
		printf("\nTotal sync grahs : %d of %zu\n", (int)cntGraphs, totalGraphs);
		double mean = sumSyncRatio / cntGraphs;
		double variance = cntGraphs == 1 ? 0 : (sumSqrSyncRatio - sumSyncRatio * sumSyncRatio / cntGraphs) / (cntGraphs - 1);
		double stdDeviation = sqrt(variance);
		printf("Sync ratio Mean     : %lf\n", mean);
		printf("Sync ratio Variance : %lf\n", variance);
		printf("Sync ratio Std Dev  : %lf\n", stdDeviation);
		printf("Sync ratio Min      : %lf\n", minSyncRatio);
		PrintGraph(minGraph);
		printf("\n");
	}
	printf("\nTotal sync checks : %d\n", SynchronizationChecksCount);

	printf("\nTime stats\n");
	PrintTimeStatsLine("Total time", TotalTime, 0);
	PrintTimeStatsLine("Sync coloring enumeration", SyncColoringsGenerationTime, 1);
	PrintTimeStatsLine("Synchronization check", SynchronizationCheckTime, 2);
	PrintTimeStatsLine("Build automata", SynchronizationCheck_BuildAutomataTime, 3);
	PrintTimeStatsLine("Build PG graph", SynchronizationCheck_BuildPGTime, 3);
	PrintTimeStatsLine("Reachability check", SynchronizationCheck_ReachabilityCheckTime, 3);
	PrintTimeStatsLine("Sync range computation", SyncRangeComputationTime, 1);
}

int main(void) {
	//	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);


	FindMaxSyncRangeFile(R"(D:\Projects\Diploma\graphs\directed_7_3_scc.d6)", 3, 100000);
	//	FindMaxSyncRange(6, 2);
	//	FindMaxSyncRangeRandom(20, 2, 10000);
	PrintStats();

	return 0;
}
