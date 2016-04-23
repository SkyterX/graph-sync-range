#include <graph/Graph.h>
#include <algorithm>

using namespace std;

namespace graph
{
	Graph::Graph(int nVertices ) {
		edges.resize(nVertices);
	}

	void Graph::AddEdge(int from, int to) {
		edges[from].push_back(to);
	}

	int Graph::VerticesCount() const {
		return edges.size();
	}

	int Graph::OutDegree() const {
		return edges[0].size();
	}

	size_t Graph::MaxOutDegree() const {
		size_t result = 0;
		for (int v = 0; v < edges.size(); ++v) {
			result = max(result, edges[v].size());
		}
		return result;
	}
}
