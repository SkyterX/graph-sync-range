#include <CppUnitTest.h>
#include <test_tools/all-test-tools.h>
#include <graph/GraphBuilders.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace test_tools;
using namespace graph;
using namespace std;

namespace graph_algo_tests
{
	TEST_CLASS(GraphBuilders_Tests) {
	public:

		TEST_METHOD(GraphBuilders_Hypercube_Test) {

			Assert::AreEqual({{0}}, BuildHypercubeGraph(0).edges);

			Assert::AreEqual({{0, 1},{1, 0}}, BuildHypercubeGraph(1).edges);

			Assert::AreEqual({{0, 1, 2},{1, 0, 3},{2, 3, 0},{3, 2, 1}}, BuildHypercubeGraph(2).edges);
		}

		TEST_METHOD(GraphBuilders_HyperTriangular_Test) {

			Assert::AreEqual({{1},{0}}, BuildHyperTriangularGraph(0).edges);

			Assert::AreEqual({{1, 2},{0, 5},{3, 4},{2, 1},{5, 0},{4, 3}}, BuildHyperTriangularGraph(1).edges);
		}

	};
}
