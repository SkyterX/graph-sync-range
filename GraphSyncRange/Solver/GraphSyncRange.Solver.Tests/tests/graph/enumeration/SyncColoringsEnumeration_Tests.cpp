#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/enumeration/SyncColoringsEnumeration.h>
#include <test_tools/TimeLapse.hpp>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(SyncColoringsEnumeration_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(SyncColoringsEnumeration_GraphsFile_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;


			Graph6Reader reader(graphsFileName);
			reader.MoveNext();
			int k = 0;
			for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
				k = max(k, static_cast<int>(reader.Current.edges[v].size()));
			}
			int n = reader.Current.VerticesCount();
			SyncColoringsEnumerator coloringsEnumerator(n, k);

			Timer::DurationType cTime(0), vTime(0);
			vector<GraphColoring::IdType> expected, actual;
			do {
				auto& graph = reader.Current;
				for (int v = 0; v < graph.VerticesCount(); ++v) {
					while (graph.edges[v].size() < k)
						graph.AddEdge(v, v);
				}

				vTime += Timer::Duration(
					[&expected, &graph]() {
						expected = GenerateSyncColorings(graph);
					});

				coloringsEnumerator.EnumerateColoringsOf(graph);
				for (int i = 0; i < expected.size() / 2; ++i) {
					Assert::IsTrue(coloringsEnumerator.MoveNext());
				}

				actual.clear();
				coloringsEnumerator.EnumerateColoringsOf(graph);
				while (true) {
					bool hasNext = false;
					cTime += Timer::Duration(
						[&hasNext, &coloringsEnumerator]() {
							hasNext = coloringsEnumerator.MoveNext();
						});
					if (!hasNext)
						break;
					actual.push_back(coloringsEnumerator.Current);
				}
				Assert::AreEqual(expected, actual);
			}
			while (reader.MoveNext()); { }

			Message::WriteLineF("Checker   : %lld", chrono::duration_cast<ResultTime>(cTime).count());
			Message::WriteLineF("Validator : %lld", chrono::duration_cast<ResultTime>(vTime).count());
		}

		TEST_METHOD(SyncColoringsRandomEnumeration_GraphsFile_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			Graph6Reader reader(graphsFileName);
			reader.MoveNext();
			int k = 0;
			for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
				k = max(k, static_cast<int>(reader.Current.edges[v].size()));
			}
			int n = reader.Current.VerticesCount();

			Permutation::GeneratePermutations(k);
			SyncColoringsEnumerator coloringsEnumerator(n, k);
			SyncColoringsRandomEnumerator coloringsRandomEnumerator(n, k);
			coloringsRandomEnumerator.SetRandomSeed(5358812812);

			Timer::DurationType nTime(0), rTime(0);
			vector<GraphColoring::IdType> expected, actual;
			do {
				auto& graph = reader.Current;
				for (int v = 0; v < graph.VerticesCount(); ++v) {
					while (graph.edges[v].size() < k)
						graph.AddEdge(v, v);
				}

				actual.clear();
				expected.clear();
				nTime += Timer::Duration(
					[&coloringsEnumerator, &graph] {
						coloringsEnumerator.EnumerateColoringsOf(graph);
					});
				rTime += Timer::Duration(
					[&coloringsRandomEnumerator, &graph] {
						coloringsRandomEnumerator.EnumerateColoringsOf(graph);
					});
				while (true) {
					bool expectedHasNext, actualHasNext;
					nTime += Timer::Duration(
						[&coloringsEnumerator, &expectedHasNext] {
							expectedHasNext = coloringsEnumerator.MoveNext();
						});
					rTime += Timer::Duration(
						[&coloringsRandomEnumerator, &actualHasNext] {
							actualHasNext = coloringsRandomEnumerator.MoveNext();
						});

					Assert::AreEqual(expectedHasNext, actualHasNext);
					if (!expectedHasNext)
						break;
					expected.push_back(coloringsEnumerator.Current);
					actual.push_back(coloringsEnumerator.Current);
				}
				stable_sort(actual.begin(), actual.end());
				Assert::AreEqual(expected, actual);
			}
			while (reader.MoveNext()); { }

			Message::WriteLineF("Sync colorings enumeration normal : %lld ms", chrono::duration_cast<ResultTime>(nTime).count());
			Message::WriteLineF("Sync colorings enumeration random : %lld ms", chrono::duration_cast<ResultTime>(rTime).count());
		}


		TEST_METHOD(SyncColoringsOptimalEnumeration_GraphsFile_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			Graph6Reader reader(graphsFileName);
			reader.MoveNext();
			int k = 0;
			for (int v = 0; v < reader.Current.VerticesCount(); ++v) {
				k = max(k, static_cast<int>(reader.Current.edges[v].size()));
			}
			int n = reader.Current.VerticesCount();

			Permutation::GeneratePermutations(k);
			SyncColoringsEnumerator coloringsEnumerator(n, k);
			SyncColoringsOptimalEnumerator coloringsOptimalEnumerator(n, k);

			Timer::DurationType nTime(0), oTime(0);
			vector<GraphColoring::IdType> expected, actual;
			do {
				auto& graph = reader.Current;
				for (int v = 0; v < graph.VerticesCount(); ++v) {
					while (graph.edges[v].size() < k)
						graph.AddEdge(v, v);
				}

				actual.clear();
				expected.clear();
				nTime += Timer::Duration(
					[&coloringsEnumerator, &graph] {
					coloringsEnumerator.EnumerateColoringsOf(graph);
				});
				oTime += Timer::Duration(
					[&coloringsOptimalEnumerator, &graph] {
					coloringsOptimalEnumerator.EnumerateColoringsOf(graph);
				});
				while (true) {
					bool expectedHasNext, actualHasNext;
					nTime += Timer::Duration(
						[&coloringsEnumerator, &expectedHasNext] {
						expectedHasNext = coloringsEnumerator.MoveNext();
					});
					oTime += Timer::Duration(
						[&coloringsOptimalEnumerator, &actualHasNext] {
						actualHasNext = coloringsOptimalEnumerator.MoveNext();
					});

					Assert::AreEqual(expectedHasNext, actualHasNext);
					if (!expectedHasNext)
						break;
					expected.push_back(coloringsEnumerator.Current);
					actual.push_back(coloringsEnumerator.Current);
				}
				stable_sort(actual.begin(), actual.end());
				Assert::AreEqual(expected, actual);
			} while (reader.MoveNext()); { }

			Message::WriteLineF("Sync colorings enumeration normal : %lld ms", chrono::duration_cast<ResultTime>(nTime).count());
			Message::WriteLineF("Sync colorings enumeration optimal : %lld ms", chrono::duration_cast<ResultTime>(oTime).count());
		}

	};
}
