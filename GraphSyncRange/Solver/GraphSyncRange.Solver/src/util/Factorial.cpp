#include <util/Factorial.h>
#include <vector>
#include <cassert>

using namespace std;

namespace util
{
	namespace Factorial
	{
		namespace
		{
			vector<IdType> generateFactorials() {
				vector<IdType> f = {1};
				auto maxValue = numeric_limits<IdType>::max();
				IdType n = 1, nF = 1;
				while (nF <= maxValue / n) {
					nF *= n;
					f.push_back(nF);
					++n;
				}
				return f;
			}

			vector<IdType> values = generateFactorials();
		}

		IdType Of(int n) {
			assert(0 <= n && n < values.size());
			return values[n];
		}

		const vector<IdType>& All() {
			return values;
		}
	}
}
