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
#include <graph/enumeration/MultiGraphEnumeration.hpp>

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
		using Timer = Measure<chrono::nanoseconds>;
		using ResultTime = chrono::milliseconds;
		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";


		TEST_METHOD(SyncRange_GraphsFile_Test) {

			vector<int> expectedResults, actualResults;

			auto srcTime = GetSyncRangeResults<SyncRangeChecker>(expectedResults, 0);

			Message::WriteLineF("Sync range checker\t: %lld ms", chrono::duration_cast<ResultTime>(srcTime).count());
			for (int minRange = 0; minRange < 4; ++minRange) {

				auto lsrcTime = GetSyncRangeResults<LazySyncRangeChecker>(actualResults, minRange);
				Assert::AreEqual(expectedResults.size(), actualResults.size());
				for (int i = 0; i < expectedResults.size(); ++i)
				{
					auto expected = expectedResults[i];
					auto actual = actualResults[i];
					if (expected <= minRange)
						Assert::IsTrue(actual <= minRange);
					else
						Assert::AreEqual(expected, actual);
				}

				Message::WriteLineF("Min Range = %d : Lazy sync range checker\t: %lld ms", minRange, chrono::duration_cast<ResultTime>(lsrcTime).count());
			}
		}

		template <typename TChecker>
		Timer::DurationType GetSyncRangeResults(vector<int>& results, int minRange) {
			results.clear();

			Graph6Reader reader(graphsFileName);

			int k = reader.Current.MaxOutDegree();
			int n = reader.Current.VerticesCount();

			TChecker syncRangeChecker(n, k, minRange);

			auto enumerator = EnumerateMultiGraphs(k, reader);

			AperiodicityChecker aperiodicityChecker;

			Timer::DurationType srcTime(0);
			do {
				auto& graph = enumerator.Current;
				if (!aperiodicityChecker.IsAperiodic(graph))
					continue;

				srcTime += Timer::Duration(
					[&graph, &syncRangeChecker, &results]() {
						results.push_back(syncRangeChecker.CheckSyncRange(graph));
					});
			}
			while (enumerator.MoveNext());

			return srcTime;
		}

	};
}
