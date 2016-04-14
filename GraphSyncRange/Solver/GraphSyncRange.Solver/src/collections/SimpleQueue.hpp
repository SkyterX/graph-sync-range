#pragma once
#include <vector>
#include <cassert>

namespace collections
{
	template <typename TValue>
	class SimpleQueue {

		std::vector<TValue> innerQueue;
		size_t qbeg, qend;

	public:
		SimpleQueue()
			: qbeg(0), qend(0) {}

		explicit SimpleQueue(size_t size)
			: SimpleQueue() {
			Resize(size);
		}

		void Resize(size_t newSize) {
			innerQueue.resize(newSize);
		}

		void Clear() {
			qbeg = 0;
			qend = 0;
		}

		void Push(TValue value) {
			assert(qend < innerQueue.size());
			innerQueue[qend++] = value;
		}

		const TValue& Peek() const {
			assert(!IsEmpty());
			return innerQueue[qbeg];
		}

		const TValue& Pop() {
			assert(!IsEmpty());
			return innerQueue[qbeg++];
		}

		size_t Size() const {
			assert(qbeg <= qend);
			return qend - qbeg;
		}

		bool IsEmpty() const {
			return qbeg >= qend;
		}
	};
}
