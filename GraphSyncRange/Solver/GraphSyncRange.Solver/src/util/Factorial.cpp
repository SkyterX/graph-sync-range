#include <util/Factorial.h>
#include <vector>
#include <cassert>

using namespace std;

namespace util
{
	vector<Factorial::IdType> Factorial::generateFactorials() {
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

	Factorial::IdType Factorial::Of(int n) {
		assert(0 <= n && n < values.size());
		return values[n];
	}

	const vector<Factorial::IdType>& Factorial::All() {
		return values;
	}

	vector<Factorial::IdType> Factorial::values = generateFactorials();
}
