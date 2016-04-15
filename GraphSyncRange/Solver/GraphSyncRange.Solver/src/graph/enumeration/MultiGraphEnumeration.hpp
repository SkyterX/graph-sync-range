#pragma once
#include <graph/Graph.h>
#include <graph/MultiGraphColoring.h>
#include <graph/enumeration/GraphEnumeration.h>

namespace graph
{
	//	template<typename TInnerEnumerator>
	class MultiGraphEnumerator {

		using TInnerEnumerator = GraphEnumerator;

		TInnerEnumerator& innerEnumerator;
		MultiGraphColoring multiColoring;

	public:

		Graph Current;

		MultiGraphEnumerator(int outDegree, TInnerEnumerator& innerEnumerator)
			: innerEnumerator(innerEnumerator),
			  multiColoring(outDegree),
			  Current(innerEnumerator.Current) {
			multiColoring.Reset(Current);
		}

		bool MoveNext() {
			if(!multiColoring.MoveNext()) {
				if (!innerEnumerator.MoveNext())
					return false;
				Current = innerEnumerator.Current;
				multiColoring.Reset(Current);
			}
			multiColoring.Apply(Current);
			return true;
		}
	};
}
