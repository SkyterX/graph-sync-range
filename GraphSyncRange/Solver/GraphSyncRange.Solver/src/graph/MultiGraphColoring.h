#pragma once

#include <graph/Graph.h>

namespace graph
{
	class MultiGraphColoring {

		struct NodeMultiColor{
			int OutDegree;
			int LoopsCount;
			int CombinationId;
			int MultiDegree;
			
			NodeMultiColor(int outDegree = 0, int vertexDegree = 0);
			
			void Reset();
			bool MoveNext();
			void Apply(Graph& graph, int vertexId);
			const std::vector<std::vector<int>>& GetCombinations() const;

			static size_t MultiColoringsCount(const size_t& outDegree, const size_t& vertexDegree);
		};

		std::vector<NodeMultiColor> nodeColors;
		size_t outDegree;
	public:
		explicit MultiGraphColoring(size_t outDegree);

		void Reset(Graph& graph);
		bool MoveNext();
		void Apply(Graph& graph);

		static size_t MultiColoringsCount(const Graph& graph, const size_t& outDegree);
	};
}
