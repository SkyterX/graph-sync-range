#include <graph/GraphColoring.h>
#include <util/MathUtils.hpp>
#include <vector>

using namespace std;
using namespace util;

namespace graph
{
	GraphColoring::GraphColoring(int nVertices, int nEdges):nEdges(nEdges) {
		edgeColors.resize(nVertices);
	}

	GraphColoring::GraphColoring(int nVertices, int nEdges, const IdType& id):GraphColoring(nVertices, nEdges) {
		auto f = Factorial::Of(nEdges);
		auto curId = id;
		for (int i = 0; i < edgeColors.size(); ++i) {
			edgeColors[i] = LazyPermutation(curId % f);
			curId /= f;
		}
	}

	void GraphColoring::Reset() {
		fill(edgeColors.begin(), edgeColors.end(), LazyPermutation());
	}

	bool GraphColoring::NextColoring() {
		for (int v = 0; v < edgeColors.size(); ++v) {
			if (edgeColors[v].NextPermutation(nEdges))
				return true;
		}
		return false;
	}

	GraphColoring::IdType GraphColoring::GetId() const {
		IdType id = 0;
		auto f = Factorial::Of(nEdges);
		for (int i = edgeColors.size(); i > 0; --i) {
			id = id * f + edgeColors[i - 1].permutationId;
		}
		return id;
	}

	GraphColoring::IdType GraphColoring::ColoringsCount(int nVertices, int nEdges) {
		auto permutationsCount = LazyPermutation::PermutationsCount(nEdges);
		return FastPower(permutationsCount, nVertices);
	}

	Graph GraphColoring::Apply(const Graph& baseGraph) const {
		Graph targetGraph(baseGraph.VerticesCount());
		Apply(baseGraph, targetGraph);
		return targetGraph;
	}

	void GraphColoring::Apply(const Graph& baseGraph, Graph& targetGraph) const {
		int n = baseGraph.VerticesCount();
		int k = nEdges;
		for (int v = 0; v < n; ++v) {
			auto& permutation = edgeColors[v].GetPermutation(k);
			targetGraph.edges[v].resize(k);
			for (int j = 0; j < k; ++j) {
				targetGraph.edges[v][j] = baseGraph.edges[v][permutation[j]];
			}
		}
	}
}
