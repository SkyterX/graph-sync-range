#include <graph/algo/StrongConnectivity.h>
#include <graph/algo/Aperiodicity.h>
#include <util/MathUtils.hpp>
#include <cassert>
#include <algorithm>

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

namespace graph
{
	struct BooleanMatrix {

		vector<vector<bool>> matrix;

		BooleanMatrix(int n) {
			matrix.resize(n);
			for (int row = 0; row < n; ++row) {
				matrix[row].assign(n, false);
			}
		}

		bool IsPositive() const {
			for (size_t row = 0; row < matrix.size(); ++row) {
				if (find(matrix[row].begin(), matrix[row].end(), false) != matrix[row].end())
					return false;
			}
			return true;
		}

		friend BooleanMatrix operator*(const BooleanMatrix& a, const BooleanMatrix& b) {
			auto n = a.matrix.size();
			BooleanMatrix c(n);
			for (size_t i = 0; i < n; ++i) {
				for (size_t j = 0; j < n; ++j) {
					for (size_t k = 0; k < n; ++k) {
						if (a.matrix[i][k] && b.matrix[k][j]) {
							c.matrix[i][j] = true;
							break;
						}
					}
				}
			}
			return c;
		}
	};

	bool AperiodicityChecker_Matrix::IsAperiodic(const Graph& graph) {
		assert(StrongConnectivityChecker_Simple().IsStronglyConnected(graph));

		int n = graph.VerticesCount();
		BooleanMatrix A(n);
		for (int v = 0; v < n; ++v) {
			for (auto& to : graph.edges[v]) {
				A.matrix[v][to] = true;
			}
		}
		// Wielandt's theorem
		return FastPower(A, n * n - 2 * n + 2).IsPositive();
	}
}
