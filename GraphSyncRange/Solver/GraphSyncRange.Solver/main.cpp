#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <cassert>
#include <util/util-all.h>
#include <graph/graph-all.h>

using namespace std;

using namespace graph;

class GraphEnumerator {

	int n, k;
	vector<vector<int>> edgeVariants;
	vector<int> currentId;

public:
	Graph Current;

	GraphEnumerator(int nVertices, int outDegree)
		:n(nVertices), k(outDegree), Current(nVertices) {
		currentId.assign(n, 0);
		GenerateEdgeVariants();
		for (int v = 0; v < n; ++v) {
			Current.edges[v] = edgeVariants[0];
		}
	}

	bool MoveNext() {
		for (int v = 0; v < n; ++v) {
			currentId[v] = (currentId[v] + 1) % edgeVariants.size();
			Current.edges[v] = edgeVariants[currentId[v]];
			if (currentId[v] != 0)
				return true;
		}
		return false;
	}

private:
	void GenerateEdgeVariants() {
		vector<bool> v(n + k - 1, false);
		fill_n(v.begin() + k, n - 1, true);

		do {
			vector<int> edges;
			int vertex = 0;
			for (int i = 0; i < v.size(); ++i) {
				if (v[i])
					++vertex;
				else
					edges.push_back(vertex);
			}
			edgeVariants.push_back(edges);
		}
		while (next_permutation(v.begin(), v.end()));
	}
};


class SimpleGraphEnumerator {

	int n, k;
	vector<vector<int>> edgeVariants;
	vector<int> currentId;

public:
	Graph Current;

	SimpleGraphEnumerator(int nVertices, int outDegree)
		:n(nVertices), k(outDegree), Current(nVertices) {
		currentId.assign(n, 0);
		GenerateEdgeVariants();
		for (int v = 0; v < n; ++v) {
			Current.edges[v] = edgeVariants[0];
		}
	}

	bool MoveNext() {
		for (int v = 0; v < n; ++v) {
			currentId[v] = (currentId[v] + 1) % edgeVariants.size();
			Current.edges[v] = edgeVariants[currentId[v]];
			if (currentId[v] != 0)
				return true;
		}
		return false;
	}

private:
	void GenerateEdgeVariants() {
		vector<bool> v(n, false);
		fill_n(v.begin(), k, true);

		do {
			vector<int> edges;
			for (int i = 0; i < v.size(); ++i) {
				if (v[i])
					edges.push_back(i);
			}
			edgeVariants.push_back(edges);
		}
		while (prev_permutation(v.begin(), v.end()));
	}
};

int main(void) {
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	//	int n, k;
	//	cin >> n >> k;
	//	n = 3;
	//	k = 2;

	auto g = ReadGraph();
	int n = g->VerticesCount();
	int k = g->OutDegree();

	util::Permutation::Generate(k);
	FindSyncRange(*g);
	//
	//	auto enumerator = SimpleGraphEnumerator(n, k);
	//	do {
	//		auto& graph = enumerator.Current;
	//		if (StrongConnectivityChecker(graph).Check()) {
	//			FindSyncRange(graph);
	//		}
	//	}
	//	while (enumerator.MoveNext());
//
//	if (cntGraphs > 0) {
//		printf("\nTotal sync grahs : %d\n", (int)cntGraphs);
//		double mean = sumSyncRatio / cntGraphs;
//		double variance = (sumSqrSyncRatio - sumSyncRatio * sumSyncRatio / cntGraphs) / (cntGraphs - 1);
//		double stdDeviation = sqrt(variance);
//		printf("Sync ratio Mean     : %lf\n", mean);
//		printf("Sync ratio Variance : %lf\n", variance);
//		printf("Sync ratio Std Dev  : %lf\n", stdDeviation);
//		printf("Sync ratio Min      : %lf\n", minSyncRatio);
//		PrintGraph(minGraph);
//	}

	return 0;
}
