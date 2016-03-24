#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <cassert>
#include <util/utils.h>
#include <util/MathUtils.hpp>

using namespace std;

namespace graph
{
	struct Graph {
		vector<vector<int>> edges;

		explicit Graph(int nVertices) {
			edges.resize(nVertices);
		}

		void AddEdge(int from, int to) {
			edges[from].push_back(to);
		}

		int VerticesCount() const {
			return edges.size();
		}

		int OutDegree() const {
			return edges[0].size();
		}
	};

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

	struct GraphColoring {
		using IdType = int;

		vector<util::LazyPermutation> edgeColors;
		int nEdges;

		GraphColoring(int nVertices, int nEdges)
			:nEdges(nEdges) {
			edgeColors.resize(nVertices);
		}

		GraphColoring(int nVertices, int nEdges, const IdType& id)
			:GraphColoring(nVertices, nEdges) {
			auto f = util::Factorial::Of(nEdges);
			auto curId = id;
			for (int i = 0; i < edgeColors.size(); ++i) {
				edgeColors[i] = util::LazyPermutation(curId % f);
				curId /= f;
			}
		}

		bool NextColoring() {
			for (int v = 0; v < edgeColors.size(); ++v) {
				if (edgeColors[v].NextPermutation(nEdges))
					return true;
			}
			return false;
		}

		IdType GetId() const {
			IdType id = 0;
			auto f = util::Factorial::Of(nEdges);
			for (int i = edgeColors.size(); i > 0; --i) {
				id = id * f + edgeColors[i - 1].permutationId;
			}
			return id;
		}

		static IdType ColoringsCount(int nVertices, int nEdges) {
			auto permutationsCount = util::LazyPermutation::PermutationsCount(nEdges);
			return util::FastPower(permutationsCount, nVertices);
		}
	};

	struct StrongConnectivityChecker {

		const Graph& graph;

		explicit StrongConnectivityChecker(const Graph& graph)
			: graph(graph) {}

		bool Check() {
			int n = graph.VerticesCount();
			depth.assign(n, -1);
			fup.assign(n, -1);

			dfs(0);
			for (int v = 0; v < n; ++v) {
				if (fup[v] != 0)
					return false;
			}
			return true;
		}

	private:
		vector<int> depth;
		vector<int> fup;

		void dfs(int v, int curDepth = 0) {
			depth[v] = curDepth;
			fup[v] = curDepth;
			for (auto& to : graph.edges[v]) {
				if (depth[to] != -1)
					fup[v] = min(fup[v], depth[to]);
				else {
					dfs(to, curDepth + 1);
					fup[v] = min(fup[v], fup[to]);
				}
			}
		}
	};

	bool IsEulerian(const Graph& graph) {
		if (graph.OutDegree() % 2 == 1) return false;
		int n = graph.VerticesCount();
		vector<int> inDegree(n, 0);
		for (int v = 0; v < n; ++v) {
			for (auto& to : graph.edges[v]) {
				++inDegree[to];
			}
		}
		for (int v = 0; v < n; ++v) {
			if (inDegree[v] % 2 == 1)
				return false;
		}
		return true;
	}

	class SynchronizationChecker {
		queue<int> q;
		vector<bool> used;
		int n, k;
		int targetNodesCount;
		Graph automata;

		// Inversed P^2(A) graph of tuples and singletons
		// For simplicity of indexing tuples, graph is of double size than needed
		Graph pg;

	public:

		explicit SynchronizationChecker(int nVertices, int outDegree)
			: n(nVertices), k(outDegree),
			  targetNodesCount(nVertices * (nVertices + 1) / 2),
			  automata(nVertices),
			  pg(nVertices * (nVertices + 1)) {
			used.resize(pg.VerticesCount());
			for (int v = 0; v < n; ++v) {
				automata.edges[v].resize(k);
			}
		}


		bool IsSynchronizing(const Graph& graph, const GraphColoring& coloring) {
			Clear();
			BuildAutomata(graph, coloring);
			BuildPGraph();
			return CheckReachability();
		}

	private:
		void Clear() {
			// Clear graph and queue
			for (int v = 0; v < pg.VerticesCount(); ++v) {
				pg.edges[v].clear();
			}
			while (!q.empty())
				q.pop();
		}

		void BuildAutomata(const Graph& graph, const GraphColoring& coloring) {
			for (int v = 0; v < n; ++v) {
				auto permutation = coloring.edgeColors[v].GetPermutation(k);
				for (int j = 0; j < k; ++j) {
					automata.edges[v][j] = graph.edges[v][permutation[j]];
				}
			}
		}

		void BuildPGraph() {
			for (int v = 0; v < n; ++v) {
				// singleton vertex is tuple (v, v) and has index v * n + v
				int w = v * n + v;
				for (int i = 0; i < k; ++i) {
					// Inversing singleton edges
					pg.AddEdge(automata.edges[v][i], w);
				}
			}

			for (int v = 0; v < n - 1; ++v) {
				for (int u = v + 1; u < n; ++u) {
					// tuple vertex (v, u) has index v*n + u
					int w = v * n + u;
					for (int i = 0; i < k; ++i) {
						int vTo = automata.edges[v][i];
						int uTo = automata.edges[u][i];
						// min/max used because tuples are unordered
						int wTo = min(vTo, uTo) * n + max(vTo, uTo);
						// Inversing tuple edge (v, u) -> (vTo, uTo)
						pg.AddEdge(wTo, w);
					}
				}
			}
		}

		bool CheckReachability() {
			// Checking that every node in P^2(A) is reachable from singletons using BFS
			int visitedCount = 0;
			used.assign(pg.VerticesCount(), false);
			for (int v = 0; v < n; ++v) {
				// Start BFS from singletons
				int w = v * n + v;
				q.push(w);
				used[w] = true;
				++visitedCount;
			}

			while (visitedCount < targetNodesCount && !q.empty()) {
				int v = q.front();
				q.pop();
				for (auto& to : pg.edges[v]) {
					if (used[to]) continue;
					q.push(to);
					used[to] = true;
					++visitedCount;
				}
			}

			// actual number of nodes
			return visitedCount == targetNodesCount;
		}
	};

	vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph) {
		vector<GraphColoring::IdType> syncColorings;
		int n = graph.VerticesCount();
		int k = graph.OutDegree();
		auto coloring = GraphColoring(n, k);
		auto syncChecker = SynchronizationChecker(n, k);
		GraphColoring::IdType id = 0;
		do {
			if (syncChecker.IsSynchronizing(graph, coloring)) {
				syncColorings.push_back(id);
			}
			++id;
		}
		while (coloring.NextColoring());
		return syncColorings;
	}

	void PrintPermutation(const util::LazyPermutation& lazyPermutation, int pSize) {
		printf(" (%d :", lazyPermutation.permutationId);
		auto p = lazyPermutation.GetPermutation(pSize);
		for (auto x : p) {
			printf(" %d", x);
		}
		printf(") ");
	}

	void PrintColoring(const GraphColoring& coloring) {
		printf(" {%d :", coloring.GetId());
		for (auto& edgeColor : coloring.edgeColors) {
			PrintPermutation(edgeColor, coloring.nEdges);
		}
		printf("} ");
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

	Graph minGraph(0);
	double minSyncRatio = 1.0;
	double sumSyncRatio = 0.0;
	double sumSqrSyncRatio = 0.0;
	double cntGraphs = 0;

	void FindSyncRange(const Graph& graph) {

		//		PrintGraph(graph);
		//		printf(" -> ");
		using ColoringIdType = GraphColoring::IdType;

		int n = graph.VerticesCount();
		int k = graph.OutDegree();

		auto totalColoringsCount = GraphColoring::ColoringsCount(n, k);
		auto syncColoringIds = GenerateSyncColorings(graph);

		if (syncColoringIds.size() == 0) {
			//			printf("No sync colorings found\n");
			return;
		}
		double syncRatio = (0.0 + syncColoringIds.size()) / totalColoringsCount;
		if (syncRatio < minSyncRatio) {
			minGraph = graph;
			minSyncRatio = syncRatio;
		}
		sumSyncRatio += syncRatio;
		sumSqrSyncRatio += syncRatio * syncRatio;
		++cntGraphs;

		if (syncColoringIds.size() == totalColoringsCount) {
			//			printf("All colorings are sync\n");
			return;
		}

		vector<int> distance(totalColoringsCount, -1);
		vector<ColoringIdType> parent(totalColoringsCount);
		queue<ColoringIdType> q;

		ColoringIdType coloringsProcessed = 0;;
		for (auto& coloringId : syncColoringIds) {
			q.push(coloringId);
			distance[coloringId] = 0;
			parent[coloringId] = coloringId;
			++coloringsProcessed;
		}

		int farthestColoringId = -1;
		while (coloringsProcessed < totalColoringsCount && !q.empty()) {
			auto coloringId = q.front();
			q.pop();

			auto coloring = GraphColoring(n, k, coloringId);
			// generate neighbors
			for (int v = 0; v < n; ++v) { // select vertex
				auto originalPermutation = coloring.edgeColors[v]; // save original coloring
				auto permutation = originalPermutation.GetPermutation(k);
				for (int a = 0; a < k - 1; ++a) { // select first edge
					for (int b = a + 1; b < k; ++b) { // select second edge
						swap(permutation[a], permutation[b]); // recolor
						coloring.edgeColors[v] = util::LazyPermutation(permutation);
						swap(permutation[a], permutation[b]); // restore 
						auto toId = coloring.GetId(); // generated neighbor

						// bfs step
						if (distance[toId] != -1) continue;
						distance[toId] = distance[coloringId] + 1;
						parent[toId] = coloringId;
						q.push(toId);
						++coloringsProcessed;

						farthestColoringId = toId;
					}
				}
				coloring.edgeColors[v] = originalPermutation; // restore original coloring of vertex
			}
		}

		assert(farthestColoringId != -1); // neighbors generation failed

		if (distance[farthestColoringId] < 2) return;
		PrintGraph(graph);
		printf(" -> ");
		printf("MaxDistance : %d from ", distance[farthestColoringId]);
		PrintColoring(GraphColoring(n, k, farthestColoringId));
		auto coloringId = farthestColoringId;
		while (parent[coloringId] != coloringId) {
			coloringId = parent[coloringId];
		}
		printf(" to ");
		PrintColoring(GraphColoring(n, k, coloringId));
		printf("\n");
	}
}

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

	return 0;
}
