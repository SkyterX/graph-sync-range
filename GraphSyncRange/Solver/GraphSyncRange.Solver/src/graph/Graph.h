#pragma once
#include <vector>

namespace graph
{
	struct Graph {
		std::vector<std::vector<int>> edges;

		explicit Graph(int nVertices);

		void AddEdge(int from, int to);

		int VerticesCount() const;

		int OutDegree() const;
	};
}
