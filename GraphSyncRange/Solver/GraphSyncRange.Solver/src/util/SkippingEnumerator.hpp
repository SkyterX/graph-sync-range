#pragma once

namespace util
{
	template <typename TEnumerator>
	struct SkippingEnumerator {
		using ValueType = typename TEnumerator::ValueType;
	private:
		TEnumerator& innerEnumerator;
		int takeEvery, skipCount;

	public:

		SkippingEnumerator(TEnumerator& innerEnumerator, int takeEvery, int skipStart = 0)
			: innerEnumerator(innerEnumerator),
			  takeEvery(takeEvery),
			  skipCount(skipStart),
			  Current(innerEnumerator.Current){
			MoveNext();
		}

		ValueType& Current;

		bool MoveNext() {
			while (skipCount-- > 0) {
				if (!innerEnumerator.MoveNext())
					return false;
			}
			Current = innerEnumerator.Current;
			skipCount = takeEvery;
			return true;
		}
	};

	template <typename TEnumerator>
	SkippingEnumerator<TEnumerator> TakeEvery(TEnumerator& innerEnumerator, int takeEvery, int skipStart) {
		return SkippingEnumerator<TEnumerator>(innerEnumerator, takeEvery, skipStart);
	}
}
