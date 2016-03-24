#pragma once
#include <util/util-all.h>
#include <vector>

namespace graph
{
	struct GraphColoring {
		using IdType = int;

		std::vector<util::LazyPermutation> edgeColors;
		int nEdges;

		GraphColoring(int nVertices, int nEdges);

		GraphColoring(int nVertices, int nEdges, const IdType& id);

		bool NextColoring();

		IdType GetId() const;

		static IdType ColoringsCount(int nVertices, int nEdges);
	};
}
