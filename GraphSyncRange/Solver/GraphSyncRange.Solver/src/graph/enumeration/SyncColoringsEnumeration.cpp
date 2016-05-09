#include <graph/enumeration/SyncColoringsEnumeration.h>
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <graph/algo/Synchronization.h>
#include <graph/enumeration/ColoringsGraph.h>
#include <graph/graph-stats.h>
#include <algorithm>

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
		currentColoring.Reset();
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


namespace graph
{
	// TODO : No shuffling, select random every time
	SyncColoringsRandomEnumerator::SyncColoringsRandomEnumerator(int verticesCount, int outDegree)
		: isFirst(true),
		  graph(nullptr),
		  syncChecker(verticesCount, outDegree),
		  random(random_device()()),
		  shuffleCnt(shuffleEvery) {

		colorings.reserve(GraphColoring::ColoringsCount(verticesCount, outDegree));
		GraphColoring coloring(verticesCount, outDegree);
		do {
			colorings.push_back({coloring.GetId(), coloring});
		}
		while (coloring.NextColoring());

		shuffle(colorings.begin(), colorings.end(), random);
		currentIt = colorings.begin();
		Current = currentIt->first;
	}

	void SyncColoringsRandomEnumerator::EnumerateColoringsOf(const Graph& graph) {
		this->graph = &graph;
		isFirst = true;
		if (--shuffleCnt == 0) {
			shuffle(colorings.begin(), colorings.end(), random);
			shuffleCnt = shuffleEvery;
		}
		currentIt = colorings.begin();
		Current = currentIt->first;
	}

	bool SyncColoringsRandomEnumerator::MoveNext() {
		CreateTimestamp();
		auto result = DoMoveNext();
		UpdateTimer(SyncColoringsGenerationTime);
		return result;
	}

	void SyncColoringsRandomEnumerator::SetRandomSeed(mt19937::result_type seed) {
		random.seed(seed);
	}

	bool SyncColoringsRandomEnumerator::DoMoveNext() {
		if (isFirst) {
			isFirst = false;
			if (syncChecker.IsSynchronizing(*graph, currentIt->second))
				return true;
		}
		while (++currentIt != colorings.end()) {
			if (syncChecker.IsSynchronizing(*graph, currentIt->second)) {
				Current = currentIt->first;
				return true;
			}
		}
		return false;
	}
}


namespace graph
{
	SyncColoringsOptimalEnumerator::SyncColoringsOptimalEnumerator(int verticesCount, int outDegree)
		: n(verticesCount), k(outDegree),
		  graph(nullptr),
		  syncChecker(verticesCount, outDegree),
		  coloringsGraph(BuildColoringsGraph(verticesCount, outDegree)),
		  Current(0) {

		colorings.reserve(GraphColoring::ColoringsCount(verticesCount, outDegree));
		GraphColoring coloring(n, k);
		do {
			colorings.push_back(coloring);
		}
		while (coloring.NextColoring());

		bfsQueue.Resize(colorings.size());
		bfsDistance.resize(colorings.size());
		InitializeQueue();

	}

	void SyncColoringsOptimalEnumerator::EnumerateColoringsOf(const Graph& graph) {
		this->graph = &graph;
		Current = 0;
		InitializeQueue();
	}

	bool SyncColoringsOptimalEnumerator::MoveNext() {
		CreateTimestamp();
		auto result = DoMoveNext();
		UpdateTimer(SyncColoringsGenerationTime);
		return result;
	}

	void SyncColoringsOptimalEnumerator::InitializeQueue() {
		const int defaultDistance = numeric_limits<int>::max();

		fill(bfsDistance.begin(), bfsDistance.end(), defaultDistance);
		bfsQueue.Clear();
		coloringsQueue.clear();

		for (GraphColoring::IdType coloringId = 0; coloringId < colorings.size(); ++coloringId) {
			coloringsQueue.insert({defaultDistance, coloringId});
		}
	}

	bool SyncColoringsOptimalEnumerator::DoMoveNext() {
		while (!coloringsQueue.empty()) {
			auto coloringId = coloringsQueue.begin()->second;
			coloringsQueue.erase(coloringsQueue.begin());
			if (syncChecker.IsSynchronizing(*graph, colorings[coloringId])) {
				Current = coloringId;
				BFSUpdate(coloringId);

				return true;
			}
		}
		return false;
	}

	void SyncColoringsOptimalEnumerator::BFSUpdate(ColoringIdType startColoringId) {
		bfsQueue.Clear();
		bfsQueue.Push(startColoringId);
		bfsDistance[startColoringId] = 0;

		while (!bfsQueue.IsEmpty()) {
			auto coloringId = bfsQueue.Pop();
			auto curDistance = bfsDistance[coloringId];

			for (const auto& neighborId : coloringsGraph.edges[coloringId]) {
				if (bfsDistance[neighborId] <= curDistance + 1) continue;
				auto it = coloringsQueue.find({bfsDistance[neighborId], neighborId});
				if (it != coloringsQueue.end()) {
					coloringsQueue.erase(it);
					coloringsQueue.insert({curDistance + 1, neighborId});
				}
				bfsDistance[neighborId] = curDistance + 1;
				bfsQueue.Push(neighborId);
			}
		}
	}
}


namespace graph
{
	SyncColoringsOrderedEnumerator::SyncColoringsOrderedEnumerator(int verticesCount, int outDegree, vector<GraphColoring::IdType> coloringsOrder)
		: isFirst(true),
		  graph(nullptr),
		  syncChecker(verticesCount, outDegree){
		colorings.reserve(coloringsOrder.size());
		for (auto& coloringId : coloringsOrder) {
			colorings.push_back({coloringId, GraphColoring(verticesCount, outDegree, coloringId)});
		}
		currentIt = colorings.begin();
		Current = currentIt->first;
	}

	void SyncColoringsOrderedEnumerator::EnumerateColoringsOf(const Graph& graph) {
		this->graph = &graph;
		isFirst = true;
		currentIt = colorings.begin();
		Current = currentIt->first;
	}

	bool SyncColoringsOrderedEnumerator::MoveNext() {
		CreateTimestamp();
		auto result = DoMoveNext();
		UpdateTimer(SyncColoringsGenerationTime);
		return result;
	}

	bool SyncColoringsOrderedEnumerator::DoMoveNext() {
		if (isFirst) {
			isFirst = false;
			if (syncChecker.IsSynchronizing(*graph, currentIt->second))
				return true;
		}
		while (++currentIt != colorings.end()) {
			if (syncChecker.IsSynchronizing(*graph, currentIt->second)) {
				Current = currentIt->first;
				return true;
			}
		}
		return false;
	}

	SyncColoringsOrderedEnumerator BuildOptimalSyncColoringsEnumerator(int verticesCount, int outDegree) {
		
		const double aging = 0.99999;
		const double edgeMultiplier = 0.99;
		using IdType = GraphColoring::IdType;

		auto coloringsGraph = BuildColoringsGraph(verticesCount, outDegree);
		auto coloringsCount = GraphColoring::ColoringsCount(verticesCount, outDegree);
		vector<IdType> order;
		order.reserve(coloringsCount);
		vector<bool> usedColorings(coloringsCount), visited;
		vector<double> score(coloringsCount, 0), updateScore;
		collections::SimpleQueue<IdType> queue;
		queue.Resize(coloringsCount);

		while (order.size() < coloringsCount) {

			IdType bestColoringId = 0;
			for (IdType id = 1; id < coloringsCount; ++id) {
				if (usedColorings[id]) continue;
				if (usedColorings[bestColoringId] || score[id] < score[bestColoringId])
					bestColoringId = id;
			}

			usedColorings[bestColoringId] = true;
			order.push_back(bestColoringId);

			updateScore.assign(coloringsCount, 0);
			visited.assign(coloringsCount, false);
			queue.Clear();
			queue.Push(bestColoringId);
			visited[bestColoringId] = true;
			updateScore[bestColoringId] = 1.0;
			score[bestColoringId] += updateScore[bestColoringId];

			IdType visitedCnt = 1;
			while (visitedCnt < coloringsCount && !queue.IsEmpty()) {
				auto id = queue.Pop();
				for (auto& to : coloringsGraph.edges[id]) {
					if (visited[to]) continue;
					visited[to] = true;
					updateScore[to] = updateScore[id] * edgeMultiplier;
					score[to] = score[to] * aging + updateScore[to];
					queue.Push(to);
					++visitedCnt;
				}
			}
		}

		return SyncColoringsOrderedEnumerator(verticesCount, outDegree, order);
	}
}
