#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/SynchronizingWord.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(SynchronizingWord_Tests) {
	public:

		static constexpr int CernyGraphSize = 7;
		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(SynchronizingWord_BruteForceBuilder_Cerny_Test) {
			const int n = CernyGraphSize;
			const int k = 2;

			Graph cernyGraph(n);
			cernyGraph.AddEdge(0, 1);
			cernyGraph.AddEdge(0, 1);
			for (int v = 1; v < n; ++v) {
				cernyGraph.AddEdge(v, (v + 1) % n);
				cernyGraph.AddEdge(v, v);
			}
			GraphColoring cernyColoring(n, k);

			ShortestSynchronizingWordBuilder_BruteForce builder(n, k);
			auto syncWord = builder.FindSynchronizingWord(cernyGraph, cernyColoring);

			Assert::IsTrue(IsSynchronizingWord(syncWord, cernyGraph, cernyColoring));
			Assert::AreEqual((n - 1) * (n - 1), static_cast<int>(syncWord.size()));
		}

		TEST_METHOD(SynchronizingWord_BruteForceBuilder_GraphsFile_Test) {

			Graph6Reader reader(graphsFileName);

			int k = reader.Current.MaxOutDegree();
			int n = reader.Current.VerticesCount();

			Permutation::GeneratePermutations(k);

			ShortestSynchronizingWordBuilder_BruteForce builder(n, k);
			GraphColoring coloring(n, k);

			double sumLength = 0;
			int totalGraphs = 0;
			do {
				auto& graph = reader.Current;
				for (int v = 0; v < graph.VerticesCount(); ++v) {
					while (graph.edges[v].size() < k)
						graph.AddEdge(v, v);
				}

				auto syncWord = builder.FindSynchronizingWord(graph, coloring);
				if (syncWord.size() == 0) continue;
				++totalGraphs;
				sumLength += syncWord.size();
				Assert::IsTrue(IsSynchronizingWord(syncWord, graph, coloring));
			}
			while (reader.MoveNext());

			Message::WriteLineF("Average sync-word length : %lf", sumLength / max(totalGraphs, 1));
		}
	};
}
