#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <map>
#include <cassert>

using namespace std;

namespace util
{
	class Factorial {
	public:
		using IdType = int;

	private:

		static vector<IdType> generateFactorials() {
			vector<IdType> f = {1};
			auto maxValue = numeric_limits<IdType>::max();
			IdType n = 1, nF = 1;
			while (nF <= maxValue / n) {
				nF *= n;
				f.push_back(nF);
				++n;
			}
			return f;
		}

		static vector<IdType> values;

	public:

		static IdType Of(int n) {
			return values[n];
		}
	};

	vector<Factorial::IdType> Factorial::values = generateFactorials();

	template <typename TVal>
	TVal FastPower(TVal val, int p) {
		if (p == 0) return 1;
		if (p == 1) return val;
		if (p % 2 == 1)
			return val * FastPower(val * val, p / 2);
		return FastPower(val * val, p / 2);
	}

	class Permutation {
	public:
		using IdType = Factorial::IdType;

	private:

		static vector<vector<vector<int>>> permutations;
		static vector<map<vector<int>, IdType>> permutationIds;
	public:

		static void Generate(int pSize) {
			if (permutations.size() < pSize + 1) {
				permutations.resize(pSize + 1);
				permutationIds.resize(pSize + 1);
			}

			vector<int> permutation(pSize);
			for (int i = 0; i < pSize; ++i) {
				permutation[i] = i;
			}

			permutations[pSize].reserve(Factorial::Of(pSize));
			int id = 0;
			do {
				permutations[pSize].push_back(permutation);
				permutationIds[pSize][permutation] = id;
				++id;
			}
			while (next_permutation(permutation.begin(), permutation.end()));
		}

		static const vector<int>& ById(int pSize, int id) {
			return permutations[pSize][id];
		}

		static IdType GetId(const vector<int>& p) {
			return permutationIds[p.size()][p];
		}
	};

	vector<vector<vector<int>>> Permutation::permutations;
	vector<map<vector<int>, Permutation::IdType>> Permutation::permutationIds;

	struct LazyPermutation {
		using IdType = Factorial::IdType;

		IdType permutationId;

		explicit LazyPermutation(IdType permutation_id = 0)
			: permutationId(permutation_id) {}

		explicit LazyPermutation(const vector<int>& p) {
			permutationId = Permutation::GetId(p);
			return;
		}

		vector<int> GetPermutation(int pSize) const {
			return Permutation::ById(pSize, permutationId);
		}

		bool NextPermutation(int pSize) {
			permutationId = (permutationId + 1) % Factorial::Of(pSize);
			return permutationId != 0;
		}

		static IdType PermutationsCount(int pSize) {
			return Factorial::Of(pSize);
		}
	};
}

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

	struct Automata : public Graph {
		Automata(const Graph& graph, const GraphColoring& coloring)
			: Graph(graph.VerticesCount()) {
			int n = graph.VerticesCount();
			int k = graph.OutDegree();
			for (int i = 0; i < n; ++i) {
				auto permutation = coloring.edgeColors[i].GetPermutation(k);
				edges[i].resize(k);
				for (int j = 0; j < k; ++j) {
					edges[i][j] = graph.edges[i][permutation[j]];
				}
			}
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

	bool IsSynchronizing(const Automata& a) {
		int n = a.VerticesCount();
		int k = a.OutDegree();

		// Inversed P^2(A) graph of tuples and singletons
		// For simplicity of indexing tuples, graph is of double size than needed
		auto pg = Graph(n * (n + 1));

		for (int v = 0; v < n; ++v) {
			// singleton vertex is tuple (v, v) and has index v * n + v
			int w = v * n + v;
			for (int i = 0; i < k; ++i) {
				// Inversing singleton edges
				pg.AddEdge(a.edges[v][i], w);
			}
		}

		for (int v = 0; v < n - 1; ++v) {
			for (int u = v + 1; u < n; ++u) {
				// tuple vertex (v, u) has index v*n + u
				int w = v * n + u;
				for (int i = 0; i < k; ++i) {
					int vTo = a.edges[v][i];
					int uTo = a.edges[u][i];
					// min/max used because tuples are unordered
					int wTo = min(vTo, uTo) * n + max(vTo, uTo);
					// Inversing tuple edge (v, u) -> (vTo, uTo)
					pg.AddEdge(wTo, w);
				}
			}
		}

		// Checking that every node in P^2(A) is reachable from singletons using BFS
		queue<int> q;
		vector<bool> used(pg.VerticesCount());
		int visitedCount = 0;
		for (int v = 0; v < n; ++v) {
			// Start BFS from singletons
			int w = v * n + v;
			q.push(w);
			used[w] = true;
			++visitedCount;
		}

		while (!q.empty()) {
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
		return visitedCount == n * (n + 1) / 2;
	}

	vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph) {
		vector<GraphColoring::IdType> syncColorings;
		int n = graph.VerticesCount();
		int k = graph.OutDegree();
		auto coloring = GraphColoring(n, k);
		GraphColoring::IdType id = 0;
		do {
			auto automata = Automata(graph, coloring);
			if (IsSynchronizing(automata)) {
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

void print(const vector<int>& v) {
	for (int i = 0; i < v.size(); ++i) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

bool NextGraph(Graph& graph) {
	int n = graph.VerticesCount();
	int k = graph.OutDegree();
	for (int v = 0; v < n; ++v) {
		for (int e = 0; e < k; ++e) {
			graph.edges[v][e] = (graph.edges[v][e] + 1) % n;
			if (graph.edges[v][e] != 0)
				return true;
		}
	}
	return false;
}

unique_ptr<Graph> StartGraph(int nVertices, int outDegree) {
	auto g = make_unique<Graph>(nVertices);
	for (int v = 0; v < nVertices; ++v) {
		for (int e = 0; e < outDegree; ++e) {
			g->AddEdge(v, 0);
		}
	}
	return g;
}

int main(void) {
	//	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	int n, k;
	cin >> n >> k;
//	n = 4;
//	k = 2;

	util::Permutation::Generate(k);
	auto g = StartGraph(n, k);
	do {
		if (StrongConnectivityChecker(*g).Check()) {
			FindSyncRange(*g);
		}
	}
	while (NextGraph(*g));

	return 0;
}
