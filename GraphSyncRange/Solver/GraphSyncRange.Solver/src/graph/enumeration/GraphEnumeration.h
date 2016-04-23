#pragma once
#include <graph/Graph.h>
#include <random>

namespace graph
{
	class GraphEnumerator {
		int n, k;
		std::vector<std::vector<int>> edgeVariants;
		std::vector<int> currentId;
		size_t totalGraphs, graphsEnumerated;

	public:
		Graph Current;

		GraphEnumerator(int nVertices, int outDegree);

		bool MoveNext();

	private:
		void GenerateEdgeVariants();
	};

	class SimpleGraphEnumerator {
		int n, k;
		std::vector<std::vector<int>> edgeVariants;
		std::vector<int> currentId;
		size_t totalGraphs, graphsEnumerated;

	public:
		Graph Current;

		SimpleGraphEnumerator(int nVertices, int outDegree);

		bool MoveNext();

	private:
		void GenerateEdgeVariants();
	};

	class RandomGraphGenerator {

		int n, k;
		std::mt19937 generator;
		std::uniform_int_distribution<int> vertexDistribution;
	public:
		Graph Current;

		RandomGraphGenerator(int verticesCount, int outDegree);

		bool MoveNext();
	};
}
