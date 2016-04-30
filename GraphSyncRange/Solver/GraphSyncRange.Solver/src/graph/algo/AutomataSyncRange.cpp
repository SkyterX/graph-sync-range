#include <graph/algo/AutomataSyncRange.h>
#include <graph/algo/Synchronization.h>
#include <graph/enumeration/GraphColoringNeighborsEnumeration.h>
#include <queue>
#include <map>
#include <cassert>
#include <unordered_map>

using namespace std;


namespace graph
{
	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct ColoringHasher {
		size_t operator() (const GraphColoring& x) const {
			size_t result = 0;
			for (auto& color : x.edgeColors) {
				hash_combine(result, color.permutationId);
			}
			return result;
		}
	};

	struct ColoringEqualityComparer {
		bool operator()(const GraphColoring& x, const GraphColoring& y) const {
			for (size_t i = 0; i < x.edgeColors.size(); ++i) {
				if (x.edgeColors[i].permutationId != y.edgeColors[i].permutationId)
					return false;
			}
			return true;
		}
	};

	struct ColoringComparer {
		bool operator()(const GraphColoring& x, const GraphColoring& y) const {
			for (size_t i = 0; i < x.edgeColors.size(); ++i) {
				if (x.edgeColors[i].permutationId != y.edgeColors[i].permutationId)
					return x.edgeColors[i].permutationId < y.edgeColors[i].permutationId;
			}
			return false;
		}
	};

	int FindSyncRange(const Graph& graph, const GraphColoring& coloring) {
		auto n = graph.VerticesCount();
		auto k = graph.OutDegree();
		SynchronizationChecker syncChecker(n, k);

		if (syncChecker.IsSynchronizing(graph, coloring))
			return 0;

		queue<GraphColoring> q;
//		map<GraphColoring, int, ColoringComparer> distance;
		unordered_map<GraphColoring, int, ColoringHasher, ColoringEqualityComparer> distance;

		q.push(coloring);
		distance[coloring] = 1;
		int prevDistance = 0;
		while (!q.empty()) {
			auto topColoring = q.front();
			q.pop();
			auto curDistance = distance[topColoring];
			if(curDistance != prevDistance) {
				prevDistance = curDistance;
				fprintf(stderr, "%d\n", curDistance - 1);
			}
			GraphColoringNeighborsEnumerator enumerator(topColoring);
			do {
				auto neighbor = enumerator.Current;
				if (distance[neighbor] > 0) continue;
				if (syncChecker.IsSynchronizing(graph, neighbor))
					return curDistance;
				distance[neighbor] = curDistance + 1;
				q.push(neighbor);
			}
			while (enumerator.MoveNext());
		}
		return -1;
	}
}
