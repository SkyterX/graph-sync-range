#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/algo/StrongConnectivity.h>
#include <functional>
#include <test_tools/TimeLapse.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(StronglyConnectedComponents_Tests) {
	public:

		static constexpr char* graphsFileName = R"(TestData\directed_6_2.d6)";

		TEST_METHOD(Compare_Tarjan_KosarajuSharir_Test) {

			using Timer = Measure<chrono::nanoseconds>;
			using ResultTime = chrono::milliseconds;

			StronglyConnectedComponentsBuilder_Tarjan TarjanSCCBuilder;
			StronglyConnectedComponentsBuilder_KosarajuSharir KosarajuSharirSCCBuilder;

			function<void(const Graph&, vector<int>&)>
					tFunc = [&TarjanSCCBuilder](const Graph& g, vector<int>& result) {
						result = TarjanSCCBuilder.FindComponents(g);
					},
					ksFunc = [&KosarajuSharirSCCBuilder](const Graph& g, vector<int>& result) {
						result = KosarajuSharirSCCBuilder.FindComponents(g);
					};

			Graph6Reader reader(graphsFileName);
			vector<int> tResult, ksResult;
			Timer::DurationType tTime(0), ksTime(0);
			do {
				tTime += Timer::Duration(tFunc, reader.Current, tResult);
				ksTime += Timer::Duration(ksFunc, reader.Current, ksResult);
				CompareSCC(tResult, ksResult);
			}
			while (reader.MoveNext());

			Message::WriteLineF("Tarjan          : %lld", chrono::duration_cast<ResultTime>(tTime).count());
			Message::WriteLineF("Kosaraju-Sharir : %lld", chrono::duration_cast<ResultTime>(ksTime).count());
		}

		void CompareSCC(const vector<int>& expected, const vector<int>& actual) {
			vector<int> componentsMap;
			componentsMap.assign(expected.size(), -1);
			for (int v = 0; v < expected.size(); ++v) {
				if (componentsMap[actual[v]] != -1)
					Assert::AreEqual(componentsMap[actual[v]], expected[v]);
				componentsMap[actual[v]] = expected[v];
			}
		}

	};
}
