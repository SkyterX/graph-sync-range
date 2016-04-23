#include <vector>
#include <algorithm>
#include <graph/enumeration/GraphEnumeration.h>
#include <graph/Graph.h>
#include <util/MathUtils.hpp>
#include <random>
#include <random>
#include <random>

using namespace std;

namespace
{
	void PrintPercentage(size_t done, size_t total) {
		if (total < 100) {
			fprintf(stderr, "Processed %d\n", done);
		}
		else {
			auto percent = (total + 99) / 100;
			if (done % percent == 0)
				fprintf(stderr, "Processed %3d%%\n", done / percent);
		}
	}
}

namespace graph
{
	GraphEnumerator::GraphEnumerator(int nVertices, int outDegree)
		:n(nVertices), k(outDegree), Current(nVertices) {
		currentId.assign(n, 0);
		GenerateEdgeVariants();
		for (int v = 0; v < n; ++v) {
			Current.edges[v] = edgeVariants[0];
		}
		totalGraphs = util::FastPower(edgeVariants.size(), n);
		graphsEnumerated = 0;
	}

	bool GraphEnumerator::MoveNext() {
		++graphsEnumerated;
		PrintPercentage(graphsEnumerated, totalGraphs);

		for (int v = 0; v < n; ++v) {
			currentId[v] = (currentId[v] + 1) % edgeVariants.size();
			Current.edges[v] = edgeVariants[currentId[v]];
			if (currentId[v] != 0)
				return true;
		}
		return false;
	}

	void GraphEnumerator::GenerateEdgeVariants() {
		vector<bool> v(n + k - 1, false);
		fill_n(v.begin() + k, n - 1, true);

		do {
			vector<int> edges;
			int vertex = 0;
			for (int i = 0; i < v.size(); ++i) {
				if (v[i])
					++vertex;
				else
					edges.push_back(vertex);
			}
			edgeVariants.push_back(edges);
		}
		while (next_permutation(v.begin(), v.end()));
	}
}

namespace graph
{
	SimpleGraphEnumerator::SimpleGraphEnumerator(int nVertices, int outDegree)
		:n(nVertices), k(outDegree), Current(nVertices) {
		currentId.assign(n, 0);
		GenerateEdgeVariants();
		for (int v = 0; v < n; ++v) {
			Current.edges[v] = edgeVariants[0];
		}
		totalGraphs = util::FastPower(edgeVariants.size(), n);
	}

	bool SimpleGraphEnumerator::MoveNext() {
		++graphsEnumerated;
		PrintPercentage(graphsEnumerated, totalGraphs);

		for (int v = 0; v < n; ++v) {
			currentId[v] = (currentId[v] + 1) % edgeVariants.size();
			Current.edges[v] = edgeVariants[currentId[v]];
			if (currentId[v] != 0)
				return true;
		}
		return false;
	}

	void SimpleGraphEnumerator::GenerateEdgeVariants() {
		vector<bool> v(n, false);
		fill_n(v.begin(), k, true);

		do {
			vector<int> edges;
			for (int i = 0; i < v.size(); ++i) {
				if (v[i])
					edges.push_back(i);
			}
			edgeVariants.push_back(edges);
		}
		while (prev_permutation(v.begin(), v.end()));
	}
}

namespace graph
{
	RandomGraphGenerator::RandomGraphGenerator(int verticesCount, int outDegree)
		: n(verticesCount), k(outDegree),
		  generator(std::random_device()()),
		  vertexDistribution(0, verticesCount - 1),
		  Current(verticesCount) {
		for (int v = 0; v < n; ++v) {
			Current.edges[v].resize(k);
		}
		MoveNext();
	}

	bool RandomGraphGenerator::MoveNext() {
		for (int v = 0; v < n; ++v) {
			for (int i = 0; i < k; ++i) {
				Current.edges[v][i] = vertexDistribution(generator);
			}
		}
		return true;
	}
}
