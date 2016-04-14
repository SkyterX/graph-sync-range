#pragma once
#include <set>
#include <vector>
#include <random>
#include <functional>
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


	class SyncColoringsOptimalEnumerator {
		using ColoringIdType = GraphColoring::IdType;
		using QueueItem = std::pair<int, ColoringIdType>;
		using QueueType = std::set<QueueItem, std::greater<QueueItem>>;


		int n, k;
		const Graph* graph;
		SynchronizationChecker syncChecker;
		std::vector<GraphColoring> colorings;
		const Graph coloringsGraph;
		collections::SimpleQueue<ColoringIdType> bfsQueue;
		std::vector<int> bfsDistance;
		QueueType coloringsQueue;

	public:

		SyncColoringsOptimalEnumerator(int verticesCount, int outDegree);
		void EnumerateColoringsOf(const Graph& graph);

		GraphColoring::IdType Current;

		bool MoveNext();

	private:
		void InitializeQueue();

		void BFSUpdate(ColoringIdType coloring_id);
		bool DoMoveNext();
	};
}
