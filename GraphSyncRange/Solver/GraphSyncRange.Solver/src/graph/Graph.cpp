#include <graph/Graph.h>

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
}
