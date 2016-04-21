#pragma once
#include <vector>

namespace util
{
	namespace Factorial
	{
		using IdType = int;

		IdType Of(int n);
		const std::vector<IdType>& All();
	};
}
