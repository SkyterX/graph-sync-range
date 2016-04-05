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
	SyncColoringsEnumerator::SyncColoringsEnumerator(const Graph& graph)
		: isFirst(true),
		  graph(graph),
		  currentColoring(graph.VerticesCount(), graph.OutDegree()),
		  syncChecker(graph.VerticesCount(), graph.OutDegree()),
		  Current(0) { }

	bool SyncColoringsEnumerator::MoveNext() {
		CreateTimestamp();
		auto result = DoMoveNext();
		UpdateTimer(SyncColoringsGenerationTime);
		return result;
	}

	bool SyncColoringsEnumerator::DoMoveNext() {
		if (isFirst) {
			isFirst = false;
			if (syncChecker.IsSynchronizing(graph, currentColoring))
				return true;
		}
		while (currentColoring.NextColoring()) {
			++Current;
			if (syncChecker.IsSynchronizing(graph, currentColoring)) {
				return true;
			}			
		}
		return false;
	}
}
