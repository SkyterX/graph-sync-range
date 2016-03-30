#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/SynchronizingWord.h>
#include <graph/algo/Synchronization.h>
#include <test_tools/TimeLapse.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(Synchronicity_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(Synchronicity_Test) {
			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			Graph6Reader reader(graphsFileName);

			reader.MoveNext();
			int k = 0;
			for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
				k = max(k, static_cast<int>(reader.Current.edges[v].size()));
			}
			int n = reader.Current.VerticesCount();

			Permutation::Generate(k);

			ShortestSynchronizingWordBuilder_BruteForce builder(n, k);
			SynchronizationChecker syncChecker(n, k);
			GraphColoring coloring(n, k);

			Timer::DurationType cTime(0), vTime(0);
			do {
				auto& graph = reader.Current;
				for (int v = 0; v < graph.VerticesCount(); ++v) {
					while (graph.edges[v].size() < k)
						graph.AddEdge(v, v);
				}

				bool expected, actual;
				vTime += Timer::Duration(
					[&builder, &coloring](const Graph& g, bool& result) {
						result = builder.FindSynchronizingWord(g, coloring).size() != 0;
					}, reader.Current, expected);
				cTime += Timer::Duration(
					[&syncChecker, &coloring](const Graph& g, bool& result) {
						result = syncChecker.IsSynchronizing(g, coloring);
					}, reader.Current, actual);
				Assert::AreEqual(expected, actual);
			}
			while (reader.MoveNext());

			Message::WriteLineF("Synchronicity Check      : %lld", chrono::duration_cast<ResultTime>(cTime).count());
			Message::WriteLineF("Build Synchronizing Word : %lld", chrono::duration_cast<ResultTime>(vTime).count());
		}
	};
}
