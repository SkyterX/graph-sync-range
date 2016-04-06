#include <graph/enumeration/SyncColoringsEnumeration.h>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <graph/algo/Synchronization.h>
#include <graph/graph-stats.h>

using namespace std;
using namespace graph::stats;

namespace graph
{
	namespace stats
	{
		TimeMeasure SyncColoringsGenerationTime(0);
	}

	vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph) {
		CreateTimestamp();

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

		UpdateTimer(SyncColoringsGenerationTime);

		return syncColorings;
	}
}

namespace graph
{
	SyncColoringsEnumerator::SyncColoringsEnumerator(int verticesCount, int outDegree)
		: isFirst(true),
		  graph(nullptr),
		  currentColoring(verticesCount, outDegree),
		  syncChecker(verticesCount, outDegree),
		  Current(0) { }

	void SyncColoringsEnumerator::EnumerateColoringsOf(const Graph& graph) {
		this->graph = &graph;
		Current = 0;
		isFirst = true;
	}

	bool SyncColoringsEnumerator::MoveNext() {
		CreateTimestamp();
		auto result = DoMoveNext();
		UpdateTimer(SyncColoringsGenerationTime);
		return result;
	}

	bool SyncColoringsEnumerator::DoMoveNext() {
		if (isFirst) {
			isFirst = false;
			if (syncChecker.IsSynchronizing(*graph, currentColoring))
				return true;
		}
		while (currentColoring.NextColoring()) {
			++Current;
			if (syncChecker.IsSynchronizing(*graph, currentColoring)) {
				return true;
			}			
		}
		return false;
	}
}
