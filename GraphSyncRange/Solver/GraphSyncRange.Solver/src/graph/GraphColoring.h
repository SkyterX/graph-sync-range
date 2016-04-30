#pragma once
#include <util/util-all.h>
#include <vector>
#include <graph/Graph.h>

namespace graph
{
	struct GraphColoring {
		using IdType = int;

		std::vector<util::LazyPermutation> edgeColors;
		int nEdges;

		GraphColoring(int nVertices, int nEdges);

		GraphColoring(int nVertices, int nEdges, const IdType& id);

		void Reset();

		bool NextColoring();

		IdType GetId() const;

		static IdType ColoringsCount(int nVertices, int nEdges);

		Graph Apply(const Graph& baseGraph) const;
		void Apply(const Graph& baseGraph, Graph& targetGraph) const;
	};
}
