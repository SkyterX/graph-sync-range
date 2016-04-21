#pragma once

#include <CppUnitTest.h>
#include <vector>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template <typename TStream, typename TElement>
			TStream& operator<<(TStream& os, const std::vector<TElement>& elements) {
				if (elements.size() == 0)
					os << "[]";
				else {
					for (size_t i = 0; i < elements.size(); ++i) {
						if (i == 0)
							os << "[";
						else
							os << ", ";
						os << elements[i];
					}
					os << "]";
				}
				return os;
			}

			template <typename T>
			inline std::wstring ToString(const std::vector<T>& elements) {
				std::wstringstream sstream;
				sstream << elements;
				return sstream.str();
			}
		}
	}
}
