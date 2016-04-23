#pragma once
#include <vector>

namespace graph
{
	struct Graph {
		std::vector<std::vector<int>> edges;

		explicit Graph(int nVertices = 0);

		void AddEdge(int from, int to);

		int VerticesCount() const;

		int OutDegree() const;

		size_t MaxOutDegree() const;
	};
}
