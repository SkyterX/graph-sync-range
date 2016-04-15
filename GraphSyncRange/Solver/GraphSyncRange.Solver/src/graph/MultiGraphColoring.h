#pragma once

#include <graph/Graph.h>

namespace graph
{
	class MultiGraphColoring {

		int outDegree;
	public:
		MultiGraphColoring(int outDegree);

		void Reset(const Graph& graph);
		bool MoveNext();
		void Apply(Graph& graph);

		static size_t MultiColoringsCount(const Graph& graph);
	};
}
