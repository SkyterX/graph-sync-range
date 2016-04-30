#include <algorithm>
#include <graph/algo/Synchronization.h>

using namespace std;
using namespace graph::stats;

namespace graph
{
	namespace stats
	{
		TimeMeasure SynchronizationCheckTime;
		TimeMeasure SynchronizationCheck_BuildAutomataTime;
		TimeMeasure SynchronizationCheck_BuildPGTime;
		TimeMeasure SynchronizationCheck_ReachabilityCheckTime;
		int SynchronizationChecksCount = 0;
	}

	// PG = Inversed P^2(A) graph of tuples and singletons
	// For simplicity of indexing tuples, graph is of double size than needed
	SynchronizationChecker::SynchronizationChecker(int nVertices, int outDegree)
		: q(nVertices * (nVertices + 1) / 2),
		  n(nVertices),
		  k(outDegree),
		  targetNodesCount(nVertices * (nVertices + 1) / 2),
		  automata(nVertices),
		  pg(nVertices * (nVertices + 1)) {
		used.resize(pg.VerticesCount());
		for (int v = 0; v < n; ++v) {
			automata.edges[v].resize(k);
		}
	}

	bool SynchronizationChecker::IsSynchronizing(const Graph& graph, const GraphColoring& coloring) {
		++SynchronizationChecksCount;
		CreateTimestamp(DefaultTimestamp);
		Clear();
		BuildAutomata(graph, coloring);
		BuildPGraph();
		auto result = CheckReachability();

		UpdateTimer(SynchronizationCheckTime);
		return result;
	}

	void SynchronizationChecker::Clear() {
		// Clear graph and queue
		for (int v = 0; v < pg.VerticesCount(); ++v) {
			pg.edges[v].clear();
		}
		q.Clear();
	}

	void SynchronizationChecker::BuildAutomata(const Graph& graph, const GraphColoring& coloring) {
		CreateTimestamp();
		coloring.Apply(graph, automata);
		UpdateTimer(SynchronizationCheck_BuildAutomataTime);
	}

	void SynchronizationChecker::BuildPGraph() {
		CreateTimestamp();
		for (int v = 0; v < n - 1; ++v) {
			for (int i = 0; i < k; ++i) {
				int vTo = automata.edges[v][i];
				for (int u = v + 1; u < n; ++u) {
					// tuple vertex (v, u) has index v*n + u
					int w = v * n + u;
					int uTo = automata.edges[u][i];
					// min/max used because tuples are unordered
					int wTo = min(vTo, uTo) * n + max(vTo, uTo);
					// Inversing tuple edge (v, u) -> (vTo, uTo)
					pg.AddEdge(wTo, w);
				}
			}
		}
		UpdateTimer(SynchronizationCheck_BuildPGTime);
	}

	bool SynchronizationChecker::CheckReachability() {
		CreateTimestamp();
		// Checking that every node in P^2(A) is reachable from singletons using BFS
		int visitedCount = 0;
		fill(used.begin(), used.end(), false);
		for (int v = 0; v < n; ++v) {
			// Start BFS from singletons
			int w = v * n + v;
			q.Push(w);
			used[w] = true;
			++visitedCount;
		}

		while (visitedCount < targetNodesCount && !q.IsEmpty()) {
			int v = q.Pop();
			for (auto& to : pg.edges[v]) {
				if (used[to]) continue;
				q.Push(to);
				used[to] = true;
				++visitedCount;
			}
		}

		UpdateTimer(SynchronizationCheck_ReachabilityCheckTime);
		// actual number of nodes
		return visitedCount == targetNodesCount;
	}
}
