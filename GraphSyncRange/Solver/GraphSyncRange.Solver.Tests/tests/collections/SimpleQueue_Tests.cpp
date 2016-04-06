#include <CppUnitTest.h>
#include <collections/SimpleQueue.hpp>
#include <queue>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace collections;

namespace util_tests
{
	TEST_CLASS(SimpleQueue_Tests) {
	public:

		TEST_METHOD(SimpleQueue_Test) {

			vector<int> values{1,2,-1,0,-1,-1,5,2,4,2,3,-1,2,4};
			SimpleQueue<int> sq(10);
			queue<int> q;

			for (auto& x : values) {
				Assert::AreEqual(q.size(), sq.Size());
				Assert::AreEqual(q.empty(), sq.IsEmpty());
				if (x < 0) {
					Assert::AreEqual(q.front(), sq.Peek());
					Assert::AreEqual(q.front(), sq.Pop());
					q.pop();
				}
				else {
					q.push(x);
					sq.Push(x);
				}
			}

			Assert::AreEqual(q.size(), sq.Size());
			while (!q.empty()) {
				Assert::AreEqual(q.front(), sq.Peek());
				Assert::AreEqual(q.front(), sq.Pop());
				q.pop();
			}

			Assert::IsTrue(sq.IsEmpty());
			sq.Clear();
			Assert::IsTrue(sq.IsEmpty());

			for (auto& x : values) {
				if (x > 0)
					sq.Push(x);
			}

			sq.Clear();
			Assert::IsTrue(sq.IsEmpty());
		}
	};
}
