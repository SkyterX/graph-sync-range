#pragma once
#include <vector>
#include <random>
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
		const Graph* graph;
		GraphColoring currentColoring;
		SynchronizationChecker syncChecker;

	public:
		SyncColoringsEnumerator(int verticesCount, int outDegree);

		void EnumerateColoringsOf(const Graph& graph);

		GraphColoring::IdType Current;

		bool MoveNext();

	private:

		bool DoMoveNext();
	};

	class SyncColoringsRandomEnumerator {

		using ColoringsCollection = std::vector<std::pair<GraphColoring::IdType, GraphColoring>>;

		bool isFirst;
		const Graph* graph;
		SynchronizationChecker syncChecker;
		std::mt19937 random;
		ColoringsCollection colorings;
		ColoringsCollection::const_iterator currentIt;

	public:
		SyncColoringsRandomEnumerator(int verticesCount, int outDegree);

		void EnumerateColoringsOf(const Graph& graph);

		GraphColoring::IdType Current;

		bool MoveNext();

		void SetRandomSeed(std::mt19937::result_type seed);

	private:

		bool DoMoveNext();
	};
}
