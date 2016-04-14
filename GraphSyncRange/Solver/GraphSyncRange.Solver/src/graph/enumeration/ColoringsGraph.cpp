#include <graph/enumeration/ColoringsGraph.h>
#include <graph/GraphColoring.h>
#include <graph/enumeration/GraphColoringNeighborsEnumeration.h>

using namespace std;

namespace graph
{
	Graph BuildColoringsGraph(int baseVerticesCount, int baseOutDegree) {
		Graph coloringsGraph(GraphColoring::ColoringsCount(baseVerticesCount, baseOutDegree));

		GraphColoring coloring(baseVerticesCount, baseOutDegree);
		do {
			const auto& coloringId = coloring.GetId();
			GraphColoringNeighborsEnumerator coloringNeighbors(coloring);
			do {
				const auto& neighborId = coloringNeighbors.Current.GetId();
				coloringsGraph.AddEdge(coloringId, neighborId);
			}
			while (coloringNeighbors.MoveNext());
		}
		while (coloring.NextColoring());

		return coloringsGraph;
	}
}
