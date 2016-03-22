#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
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

	template<typename TVal>
	TVal FastPower(TVal val, int p) {
		if (p == 0) return 1;
		if (p == 1) return val;
		if (p % 2 == 1)
			return val * FastPower(val * val, p / 2);
		return FastPower(val * val, p / 2);
	}

	struct LazyPermutation {
		using IdType = Factorial::IdType;

		IdType permutationId;

		explicit LazyPermutation(IdType permutation_id = 0)
			: permutationId(permutation_id) {}

		explicit LazyPermutation(const vector<int>& p) {
			int n = p.size();
			vector<int> pool(n);
			for (int i = 0; i < n; ++i) {
				pool[i] = i;
			}
			IdType currentId = 0;
			for (int i = 0; i < n; ++i) {
				auto it = find(pool.begin(), pool.end(), p[i]);
				int idx = it - pool.begin();
				pool.erase(it);
				currentId += idx * Factorial::Of(n - i - 1);
			}
			permutationId = currentId;
		}

		vector<int> GetPermutation(int pSize) const {
			vector<int> permutation(pSize), pool(pSize);
			for (int i = 0; i < pSize; ++i) {
				pool[i] = i;
			}
			IdType currentId = permutationId;
			for (int i = 0; i < pSize; ++i) {
				int idx = currentId / Factorial::Of(pSize - i - 1);
				permutation[i] = pool[idx];

				pool.erase(pool.begin() + idx);

				currentId %= Factorial::Of(pSize - i - 1);
			}
			return permutation;
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
		for(auto x : p) {
			printf(" %d", x);
		}
		printf(") ");
	}

	void PrintColoring(const GraphColoring& coloring) {
		printf(" {%d :", coloring.GetId());
		for(auto& edgeColor : coloring.edgeColors) {
			PrintPermutation(edgeColor, coloring.nEdges);
		}
		printf("} ");
	}

	void FindSyncRange(const Graph& graph) {
		using ColoringIdType = GraphColoring::IdType;

		int n = graph.VerticesCount();
		int k = graph.OutDegree();

		auto totalColoringsCount = GraphColoring::ColoringsCount(n, k);
		auto syncColoringIds = GenerateSyncColorings(graph);
		if(syncColoringIds.size() == 0) {
			printf("No sync colorings found\n");
			return;
		}
		if(syncColoringIds.size() == totalColoringsCount) {
			printf("All colorings are sync\n");
			return;
		}

		vector<int> distance(totalColoringsCount, -1);
		vector<ColoringIdType> parent(totalColoringsCount);
		queue<ColoringIdType> q;

		ColoringIdType coloringsProcessed = 0;;
		for(auto& coloringId : syncColoringIds) {
			q.push(coloringId);
			distance[coloringId] = 0;
			parent[coloringId] = coloringId;
			++coloringsProcessed;
		}

		int farthestColoringId = -1;
		while(coloringsProcessed < totalColoringsCount && !q.empty()) {
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

		printf("MaxDistance : %d from ", distance[farthestColoringId]);
		PrintColoring(GraphColoring(n, k, farthestColoringId));
		auto coloringId = farthestColoringId;
		while(parent[coloringId] != coloringId) {
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

int main(void) {
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	auto g = ReadGraph();
	FindSyncRange(*g);

	return 0;
}
