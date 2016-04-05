#pragma once
#include <vector>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <graph/graph-stats.h>
#include <graph/algo/Synchronization.h>

namespace graph
{
	namespace stats
	{
		extern TimeMeasure SyncColoringsGenerationTime;
	}

	std::vector<GraphColoring::IdType> GenerateSyncColorings(const Graph& graph);

	class SyncColoringsEnumerator {

		bool isFirst;
		const Graph& graph;
		GraphColoring currentColoring;
		SynchronizationChecker syncChecker;

	public:
		SyncColoringsEnumerator(const Graph& graph);

		GraphColoring::IdType Current;

		bool MoveNext();

	private:

		bool DoMoveNext();
	};
}
