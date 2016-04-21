#include <graph/MultiGraphColoring.h>
#include <util/Combinations.h>
#include <cassert>

using namespace std;
using namespace util;

namespace graph
{
	MultiGraphColoring::NodeMultiColor::NodeMultiColor(int outDegree, int vertexDegree)
		: OutDegree(outDegree),
		  LoopsCount(outDegree - vertexDegree),
		  CombinationId(0),
		  MultiDegree(0) {}

	void MultiGraphColoring::NodeMultiColor::Reset() {
		LoopsCount += MultiDegree;
		MultiDegree = 0;
		CombinationId = 0;
	}

	bool MultiGraphColoring::NodeMultiColor::MoveNext() {
		++CombinationId;
		if (CombinationId < GetCombinations().size())
			return true;
		CombinationId = 0;
		if (LoopsCount == OutDegree) return false;
		if (LoopsCount != 0) {
			--LoopsCount;
			++MultiDegree;
			return true;
		}
		swap(LoopsCount, MultiDegree);
		return false;
	}

	void MultiGraphColoring::NodeMultiColor::Apply(Graph& graph, int vertexId) {
		int edgeId = OutDegree - LoopsCount - MultiDegree;
		for (int loopId = 0; loopId < LoopsCount; ++loopId)
			graph.edges[vertexId][edgeId++] = vertexId;
		const auto& combination = GetCombinations()[CombinationId];
		for (auto& copyEdgeId : combination) {
			graph.edges[vertexId][edgeId++] = graph.edges[vertexId][copyEdgeId];
		}
	}

	const vector<vector<int>>& MultiGraphColoring::NodeMultiColor::GetCombinations() const {
		return CombinationsWithRepetitions::Of(OutDegree - MultiDegree - LoopsCount, MultiDegree);
	}

	size_t MultiGraphColoring::NodeMultiColor::MultiColoringsCount(const size_t& outDegree, const size_t& vertexDegree) {
		size_t result = 0;
		for (int i = vertexDegree; i <= outDegree; ++i) {
			result += CombinationsWithRepetitions::Count(vertexDegree, outDegree - i);
		}
		return result;
	}

	MultiGraphColoring::MultiGraphColoring(size_t outDegree)
		: outDegree(outDegree) {}

	void MultiGraphColoring::Reset(Graph& graph) {
		nodeColors.resize(graph.VerticesCount());
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			assert(graph.edges[v].size() <= outDegree);
			nodeColors[v] = NodeMultiColor(outDegree, graph.edges[v].size());
			graph.edges[v].resize(outDegree);
		}
		Apply(graph);
	}

	bool MultiGraphColoring::MoveNext() {
		for (auto& nodeColor : nodeColors) {
			if (nodeColor.MoveNext())
				return true;
		}
		return false;
	}

	void MultiGraphColoring::Apply(Graph& graph) {
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			nodeColors[v].Apply(graph, v);
		}
	}

	size_t MultiGraphColoring::MultiColoringsCount(const Graph& graph, const size_t& outDegree) {
		size_t result = 1;
		for (int v = 0; v < graph.VerticesCount(); ++v) {
			assert(graph.edges[v].size() <= outDegree);
			result *= NodeMultiColor::MultiColoringsCount(outDegree, graph.edges[v].size());
		}
		return result;
	}
}
