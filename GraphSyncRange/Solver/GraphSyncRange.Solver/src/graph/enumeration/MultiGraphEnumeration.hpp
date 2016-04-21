#pragma once
#include <graph/Graph.h>
#include <graph/MultiGraphColoring.h>

namespace graph
{
	template <typename TInnerEnumerator>
	class MultiGraphEnumerator {

		TInnerEnumerator& innerEnumerator;
		MultiGraphColoring multiColoring;

	public:

		Graph Current;

		MultiGraphEnumerator(size_t outDegree, TInnerEnumerator& innerEnumerator)
			: innerEnumerator(innerEnumerator),
			  multiColoring(outDegree),
			  Current(innerEnumerator.Current) {
			multiColoring.Reset(Current);
		}

		bool MoveNext() {
			if (!multiColoring.MoveNext()) {
				if (!innerEnumerator.MoveNext())
					return false;
				Current = innerEnumerator.Current;
				multiColoring.Reset(Current);
			}
			else
				multiColoring.Apply(Current);
			return true;
		}
	};

	template <typename TInnerEnumerator>
	MultiGraphEnumerator<TInnerEnumerator> EnumerateMultiGraphs(size_t outDegree, TInnerEnumerator& graphEnumerator) {
		return MultiGraphEnumerator<TInnerEnumerator>(outDegree, graphEnumerator);
	}
}
