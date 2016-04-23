#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/Eulerian.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(Eulerianicity_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2.d6)";

		TEST_METHOD(Eulerianicity_GraphsFile_Test) {

			Graph6Reader reader(graphsFileName);
			vector<int> vertexDegreeDiffs;
			int eulerianCount = 0, totalCount = 0;
			do {
				auto& graph = reader.Current;
				auto n = graph.VerticesCount();
				vertexDegreeDiffs.assign(n, 0);
				for (int v = 0; v < n; ++v) {
					vertexDegreeDiffs[v] += graph.edges[v].size();
					for (auto& to : graph.edges[v]) {
						--vertexDegreeDiffs[to];
					}
				}
				bool isEulerian = std::all_of(
					vertexDegreeDiffs.begin(), vertexDegreeDiffs.end(),
					[](const int x) {
						return x == 0;
					});
				Assert::AreEqual(isEulerian, IsEulerian(graph));
				eulerianCount += isEulerian;
				++totalCount;
			}
			while (reader.MoveNext());

			Message::WriteLineF("Total Eulerian graphs : %d of %d", eulerianCount, totalCount);
		}

	};
}
