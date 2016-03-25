#pragma once

#include <CppUnitTest.h>
#include <vector>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template <typename T>
			inline std::wstring ToString(const std::vector<T>& elements) {
				std::wstringstream sstream;
				for (size_t i = 0; i < elements.size(); ++i) {
					if (i == 0)
						sstream << "[";
					else
						sstream << ", ";
					sstream << elements[i];
				}
				sstream << "]";
				return sstream.str();
			}
		}
	}
}
