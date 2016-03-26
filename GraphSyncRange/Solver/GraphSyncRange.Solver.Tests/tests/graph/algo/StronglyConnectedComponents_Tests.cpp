#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/algo/GraphEnumeration.h>
#include <graph/algo/StrongConnectivity.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std;
using namespace std::experimental;

namespace graph_algo_tests
{
	TEST_CLASS(StronglyConnectedComponents_Tests) {
	public:

		static const int TestGraphSize = 5;
		static const int TestGraphDegree = 2;

		TEST_METHOD(Compare_Tarjan_KosarajuSharir_Test) {
			SimpleGraphEnumerator graphEnumerator(TestGraphSize, 2);
			StronglyConnectedComponentsBuilder_Tarjan TarjanSCCBuilder;
			StronglyConnectedComponentsBuilder_KosarajuSharir KosarajuSharirSCCBuilder;

			vector<int> componentsMap;

			do {
				auto tResult = TarjanSCCBuilder.FindComponents(graphEnumerator.Current);
				auto ksResult = KosarajuSharirSCCBuilder.FindComponents(graphEnumerator.Current);

				componentsMap.assign(TestGraphSize, -1);
				for (int v = 0; v < TestGraphSize; ++v) {
					if (componentsMap[ksResult[v]] != -1)
						Assert::AreEqual(componentsMap[ksResult[v]], tResult[v]);
					componentsMap[ksResult[v]] = tResult[v];
				}
			}
			while (graphEnumerator.MoveNext());
		}

	};
}
