#include <CppUnitTest.h>
#include <test_tools/ToStringExtensions.hpp>
#include <util/MathUtils.hpp>
#include <test_tools/Message.h>
#include <experimental/generator>
#include <graph/Graph6Reader.h>
#include <graph/GraphColoring.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace util;
using namespace std::experimental;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(GraphColoring_Tests) {
	public:

		static constexpr int TestGraphSize = 4;
		static constexpr int TestGraphDegree = 3;

		TEST_METHOD(GraphColoring_Enumeration_Test) {

			GraphColoring coloring(TestGraphSize, TestGraphDegree);
			auto prevColoring = coloring.edgeColors;
			GraphColoring::IdType coloringsEnumerated = 1;
			while (coloring.NextColoring()) {
				bool isLexicographicallyOrdered = lexicographical_compare(
					prevColoring.rbegin(), prevColoring.rend(),
					coloring.edgeColors.rbegin(), coloring.edgeColors.rend(),
					[](const LazyPermutation& x, const LazyPermutation& y) {
						return x.permutationId < y.permutationId;
					});

				Assert::IsTrue(isLexicographicallyOrdered);
				prevColoring = coloring.edgeColors;
				++coloringsEnumerated;
			}

			Assert::AreEqual(GraphColoring::ColoringsCount(TestGraphSize, TestGraphDegree), coloringsEnumerated);

			Message::WriteLineF("%d", coloringsEnumerated);

		}
	};
}
