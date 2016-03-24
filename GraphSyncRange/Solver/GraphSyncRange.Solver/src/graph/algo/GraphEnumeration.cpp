#include <vector>
#include <algorithm>
#include <graph/algo/GraphEnumeration.h>
#include <graph/Graph.h>

using namespace std;

namespace graph
{
	GraphEnumerator::GraphEnumerator(int nVertices, int outDegree)
		:n(nVertices), k(outDegree), Current(nVertices) {
		currentId.assign(n, 0);
		GenerateEdgeVariants();
		for (int v = 0; v < n; ++v) {
			Current.edges[v] = edgeVariants[0];
		}
	}

	bool GraphEnumerator::MoveNext() {
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
	}

	bool SimpleGraphEnumerator::MoveNext() {
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
