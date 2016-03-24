#include <util/Factorial.h>
#include <vector>

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
		return values[n];
	}

	vector<Factorial::IdType> Factorial::values = generateFactorials();
}
