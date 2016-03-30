#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/Aperiodicity.h>
#include <test_tools/TimeLapse.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(Aperiodicity_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(Aperiodicity_GraphsFile_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			AperiodicityChecker checker;
			AperiodicityChecker_Matrix validator;

			Timer::DurationType cTime(0), vTime(0);
			Graph6Reader reader(graphsFileName);
			while (reader.MoveNext()) {
				bool expected, actual;
				vTime += Timer::Duration(
					[&validator](const Graph& graph, bool& result) {
						result = validator.IsAperiodic(graph);
					}, reader.Current, expected);
				cTime += Timer::Duration(
					[&checker](const Graph& graph, bool& result) {
						result = checker.IsAperiodic(graph);
					}, reader.Current, actual);
				Assert::AreEqual(expected, actual);
			}

			Message::WriteLineF("Checker   : %lld", chrono::duration_cast<ResultTime>(cTime).count());
			Message::WriteLineF("Validator : %lld", chrono::duration_cast<ResultTime>(vTime).count());
		}

		TEST_METHOD(Aperiodicity_NonAperiodicGraph_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			AperiodicityChecker checker;
			AperiodicityChecker_Matrix validator;

			Graph graph(10);
			graph.AddEdge(0, 1);
			graph.AddEdge(0, 2);
			graph.AddEdge(1, 3);
			graph.AddEdge(2, 3);
			graph.AddEdge(3, 0);
			graph.AddEdge(3, 4);
			graph.AddEdge(4, 5);
			graph.AddEdge(5, 6);
			graph.AddEdge(6, 7);
			graph.AddEdge(7, 8);
			graph.AddEdge(8, 3);
			graph.AddEdge(8, 9);
			graph.AddEdge(9, 7);

			Assert::IsFalse(validator.IsAperiodic(graph), L"Validator failed");
			Assert::IsFalse(checker.IsAperiodic(graph), L"Checker failed");
		}


		TEST_METHOD(Aperiodicity_AperiodicGraph_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			AperiodicityChecker checker;
			AperiodicityChecker_Matrix validator;

			Graph graph(9);
			graph.AddEdge(0, 1);
			graph.AddEdge(1, 2);
			graph.AddEdge(2, 3);
			graph.AddEdge(3, 4);
			graph.AddEdge(4, 5);
			graph.AddEdge(5, 0);
			graph.AddEdge(0, 6);
			graph.AddEdge(6, 7);
			graph.AddEdge(7, 8);
			graph.AddEdge(8, 5);


			Assert::IsTrue(validator.IsAperiodic(graph), L"Validator failed");
			Assert::IsTrue(checker.IsAperiodic(graph), L"Checker failed");
		}

	};
}
