#include <graph/algo/StrongConnectivity.h>
#include <graph/algo/Aperiodicity.h>
#include <util/MathUtils.hpp>
#include <cassert>

using namespace std;
using namespace util;

namespace graph
{
	bool AperiodicityChecker::IsAperiodic(const Graph& graph) {
		assert(StrongConnectivityChecker_Simple().IsStronglyConnected(graph));
		this->graph = &graph;
		Clear();
		BuildDepth(0, 0);

		int kGCD = 0;
		auto n = graph.VerticesCount();
		for (int v = 0; v < n; ++v) {
			for (auto& to : graph.edges[v]) {
				int ke = abs(depth[to] - depth[v] - 1);
				kGCD = GCD(kGCD, ke);
			}
		}
		return kGCD == 1;
	}

	void AperiodicityChecker::Clear() {
		auto n = graph->VerticesCount();
		depth.assign(n, -1);
	}

	void AperiodicityChecker::BuildDepth(int v, int currentDepth) {
		depth[v] = currentDepth;
		for (auto& to : graph->edges[v]) {
			if (depth[to] == -1)
				BuildDepth(to, currentDepth + 1);
		}
	}
}
