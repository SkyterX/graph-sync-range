#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <util/Permutations.h>

namespace graph
{
	bool AreNeighbors(const GraphColoring& a, const GraphColoring& b);

	class GraphColoringNeighborsEnumerator_BruteForce {
		const GraphColoring* original;
	public:
		explicit GraphColoringNeighborsEnumerator_BruteForce(const GraphColoring& coloring);

		GraphColoring Current;

		bool MoveNext();
	};

	class GraphColoringNeighborsEnumerator {
	private:

		size_t currentVertex, firstEdgeIdx, secondEdgeIdx;
		util::LazyPermutation originalPermutation;
		std::vector<int> currentPermutation;

	public:
		explicit GraphColoringNeighborsEnumerator(const GraphColoring& coloring);

		GraphColoring Current;

		bool MoveNext();
	};
}
