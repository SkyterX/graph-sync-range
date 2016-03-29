#pragma once
#include <graph/Graph.h>
#include <graph/GraphColoring.h>
#include <queue>
#include <set>
#include <vector>

namespace graph
{
	bool IsSynchronizingWord(const std::vector<int>& word, const Graph& graph, const GraphColoring& coloring);

	class ShortestSynchronizingWordBuilder_BruteForce {

		int n, k;
		Graph automata;
		std::queue<std::pair<std::set<int>, std::vector<int>>> q;
		std::set<std::set<int>> used;

	public:

		ShortestSynchronizingWordBuilder_BruteForce(int verticesCount, int outDegree);

		std::vector<int> FindSynchronizingWord(const Graph& graph, const GraphColoring& coloring);

	private:
		void Clear();

		std::vector<int> FindSynchronizingWord();
	};
}
