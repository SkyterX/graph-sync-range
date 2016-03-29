#include <graph/algo/SynchronizingWord.h>

using namespace std;

namespace graph
{
	void BuildAutomata(const Graph& graph, const GraphColoring& coloring, Graph& automata) {
		int n = graph.VerticesCount();
		int k = graph.OutDegree();
		for (int v = 0; v < n; ++v) {
			auto permutation = coloring.edgeColors[v].GetPermutation(k);
			automata.edges[v].resize(k);
			for (int j = 0; j < k; ++j) {
				automata.edges[v][j] = graph.edges[v][permutation[j]];
			}
		}
	}
}

namespace graph
{
	bool IsSynchronizingWord(const vector<int>& word, const Graph& graph, const GraphColoring& coloring) {
		int n = graph.VerticesCount();
		Graph automata(n);
		BuildAutomata(graph, coloring, automata);

		int targetState = -1;
		for (int v = 0; v < n; ++v) {
			int state = v;
			for (auto& letter : word) {
				state = automata.edges[state][letter];
			}
			if (targetState == -1)
				targetState = state;
			if (targetState != state)
				return false;
		}
		return true;
	}
}

namespace graph
{
	ShortestSynchronizingWordBuilder_BruteForce::ShortestSynchronizingWordBuilder_BruteForce(int verticesCount, int outDegree)
		:n(verticesCount), k(outDegree), automata(verticesCount) { }

	vector<int> ShortestSynchronizingWordBuilder_BruteForce::FindSynchronizingWord(const Graph& graph, const GraphColoring& coloring) {
		Clear();
		BuildAutomata(graph, coloring, automata);
		return FindSynchronizingWord();
	}

	void ShortestSynchronizingWordBuilder_BruteForce::Clear() {
		while (!q.empty())
			q.pop();
		used.clear();
	}

	vector<int> ShortestSynchronizingWordBuilder_BruteForce::FindSynchronizingWord() {

		set<int> initial;
		for (int v = 0; v < n; ++v) {
			initial.insert(v);
		}
		q.push(make_pair(initial, vector<int>()));
		used.insert(initial);

		while (!q.empty()) {

			auto topItem = q.front();
			q.pop();
			auto& topSet = topItem.first;
			auto& topWord = topItem.second;

			for (int letter = 0; letter < k; ++letter) {
				set<int> targetSet;
				for (auto& v : topSet) {
					targetSet.insert(automata.edges[v][letter]);
				}
				if (used.find(targetSet) != used.end()) continue;
				vector<int> targetWord = topWord;
				targetWord.push_back(letter);
				if (targetSet.size() == 1)
					return targetWord;
				q.push(make_pair(targetSet, targetWord));
				used.insert(targetSet);
			}
		}

		return vector<int>();
	}
}
