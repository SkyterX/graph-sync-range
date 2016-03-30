#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/StrongConnectivity.h>
#include <test_tools/TimeLapse.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(StrongConnectivity_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2.d6)";
		static constexpr char* scGraphsFileName = R"(TestData\directed_6_2_scc.d6)";

		TEST_METHOD(SimpleChecker_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			StrongConnectivityChecker_Simple checker;
			StrongConnectivityChecker_KosarajuSharir validator;

			Timer::DurationType cTime(0), vTime(0);
			Graph6Reader reader(graphsFileName);
			while (reader.MoveNext()) {
				bool expected, actual;
				vTime += Timer::Duration(
					[&validator](const Graph& graph, bool& result) {
						result = validator.IsStronglyConnected(graph);
					}, reader.Current, expected);
				cTime += Timer::Duration(
					[&checker](const Graph& graph, bool& result) {
						result = checker.IsStronglyConnected(graph);
					}, reader.Current, actual);
				Assert::AreEqual(expected, actual);
			}

			Message::WriteLineF("Checker   : %lld", chrono::duration_cast<ResultTime>(cTime).count());
			Message::WriteLineF("Validator : %lld", chrono::duration_cast<ResultTime>(vTime).count());
		}

		TEST_METHOD(SimpleChecker_SCGraphsFile_Test) {
			StrongConnectivityChecker_Simple checker;
			Graph6Reader reader(scGraphsFileName);
			while (reader.MoveNext()) {
				Assert::IsTrue(checker.IsStronglyConnected(reader.Current));
			}
		}
	};
}
