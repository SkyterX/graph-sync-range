#include <CppUnitTest.h>
#include <graph/GraphIO.h>
#include <graph/Graph6Reader.h>
#include <test_tools/ToStringExtensions.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace graph;
using namespace std;

namespace util_tests
{
	TEST_CLASS(Graph6Reader_Tests) {
	public:

#define Graph6FileName R"(TestData\sample_graph.d6)"
#define GraphFileName R"(TestData\sample_graph.graph)"

		TEST_METHOD(Graph6Reader_Test) {
			auto graph = ReadGraph(fopen(GraphFileName, "rt"));
			auto reader = Graph6Reader(Graph6FileName);
			Assert::IsTrue(reader.MoveNext(), L"Reader failed : no graph found.");
			auto graph6 = reader.Current;
			Assert::IsFalse(reader.MoveNext(), L"Reader failed : multiple graphs found.");
			Assert::AreEqual(graph->VerticesCount(), graph6.VerticesCount(),
							 L"Wrong number of vertices");
			for (int v = 0; v < graph->VerticesCount(); ++v) {
				Assert::AreEqual(graph->edges[v], graph6.edges[v], L"Edges differ");
			}
		}

	};
}
