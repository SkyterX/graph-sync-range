#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <test_tools/TimeLapse.hpp>
#include <util/Permutations.h>
#include <graph/algo/SyncRange.h>
#include <graph/algo/LazySyncRange.h>
#include <graph/algo/Aperiodicity.h>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(SyncRange_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(SyncRange_GraphsFile_Test) {
			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			Graph6Reader reader(graphsFileName);

			reader.MoveNext();
			int k = 0;
			for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
				k = max(k, static_cast<int>(reader.Current.edges[v].size()));
			}
			int n = reader.Current.VerticesCount();

			SyncRangeChecker syncRangeChecker(n, k);
			LazySyncRangeChecker lazySyncRangeChecker(n, k);
			AperiodicityChecker aperiodicityChecker;

			Timer::DurationType srcTime(0), lsrcTime(0);
			do {
				auto& graph = reader.Current;
				for (int v = 0; v < graph.VerticesCount(); ++v) {
					while (graph.edges[v].size() < k)
						graph.AddEdge(v, v);
				}
				if (!aperiodicityChecker.IsAperiodic(graph))
					continue;

				int expected, actual;
				lsrcTime += Timer::Duration(
					[&graph, &lazySyncRangeChecker, &actual]() {
						actual = lazySyncRangeChecker.CheckSyncRange(graph);
					});
				srcTime += Timer::Duration(
					[&graph, &syncRangeChecker, &expected]() {
						expected = syncRangeChecker.CheckSyncRange(graph);
					});
				if (expected == 1)
					Assert::AreEqual(expected, max(actual, 1));
				else
					Assert::AreEqual(expected, actual);
			}
			while (reader.MoveNext());

			Message::WriteLineF("Sync range checker\t: %lld ms", chrono::duration_cast<ResultTime>(srcTime).count());
			Message::WriteLineF("Lazy sync range checker\t: %lld ms", chrono::duration_cast<ResultTime>(lsrcTime).count());
		}

	};
}
