#include <graph/enumeration/GraphColoringNeighborsEnumeration.h>
#include <cassert>

using namespace std;
using namespace util;

namespace graph
{
	bool AreNeighbors(const GraphColoring& a, const GraphColoring& b) {
		bool areDifferent = false;
		if (a.nEdges != b.nEdges)
			return false;
		for (size_t i = 0; i < a.edgeColors.size(); ++i) {
			if (a.edgeColors[i].permutationId == b.edgeColors[i].permutationId) continue;
			if (areDifferent) return false;
			areDifferent = true;
			auto& aP = a.edgeColors[i].GetPermutation(a.nEdges);
			auto& bP = b.edgeColors[i].GetPermutation(b.nEdges);
			int diffCnt = 0;
			for (size_t j = 0; j < aP.size() && diffCnt < 3; ++j) {
				diffCnt += aP[j] != bP[j] ? 1 : 0;
			}
			assert(diffCnt >= 2);
			if (diffCnt != 2) return false;
		}
		return areDifferent;
	}

	GraphColoringNeighborsEnumerator_BruteForce::GraphColoringNeighborsEnumerator_BruteForce(const GraphColoring& coloring)
		: original(&coloring), Current(coloring.edgeColors.size(), coloring.nEdges) {
		do {
			if (AreNeighbors(*original, Current))
				break;
		}
		while (Current.NextColoring());
	}

	bool GraphColoringNeighborsEnumerator_BruteForce::MoveNext() {
		while (Current.NextColoring()) {
			if (AreNeighbors(*original, Current))
				return true;
		}
		return false;
	}
}

namespace graph
{
	GraphColoringNeighborsEnumerator::GraphColoringNeighborsEnumerator(const GraphColoring& coloring)
		: currentVertex(0), firstEdgeIdx(0), secondEdgeIdx(1),
		  originalPermutation(coloring.edgeColors[0]),
		  Current(coloring) {
		currentPermutation = coloring.edgeColors[0].GetPermutation(coloring.nEdges);
		swap(currentPermutation[firstEdgeIdx], currentPermutation[secondEdgeIdx]);
		Current.edgeColors[currentVertex] = LazyPermutation(currentPermutation);
	}

	bool GraphColoringNeighborsEnumerator::MoveNext() {
		swap(currentPermutation[firstEdgeIdx], currentPermutation[secondEdgeIdx]);
		++secondEdgeIdx;
		if (secondEdgeIdx == Current.nEdges) {
			++firstEdgeIdx;
			if (firstEdgeIdx + 1 == Current.nEdges) {
				Current.edgeColors[currentVertex] = originalPermutation;
				++currentVertex;
				if (currentVertex == Current.edgeColors.size())
					return false;
				originalPermutation = Current.edgeColors[currentVertex];
				currentPermutation = originalPermutation.GetPermutation(Current.nEdges);
				firstEdgeIdx = 0;
				secondEdgeIdx = 1;
			}
			else
				secondEdgeIdx = firstEdgeIdx + 1;
		}
		swap(currentPermutation[firstEdgeIdx], currentPermutation[secondEdgeIdx]);
		Current.edgeColors[currentVertex] = LazyPermutation(currentPermutation);
		return true;
	}
}
