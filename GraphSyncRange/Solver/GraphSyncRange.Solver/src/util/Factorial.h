#pragma once
#include <vector>

namespace util
{
	class Factorial {
	public:
		using IdType = int;

	private:

		static std::vector<IdType> generateFactorials();

		static std::vector<IdType> values;

	public:

		static IdType Of(int n);
		static const std::vector<IdType>& All();
	};
}
