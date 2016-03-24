#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <util/UtilIO.h>

namespace util
{
	void PrintPermutation(const LazyPermutation& lazyPermutation, int pSize) {
		printf(" (%d :", lazyPermutation.permutationId);
		auto p = lazyPermutation.GetPermutation(pSize);
		for (auto x : p) {
			printf(" %d", x);
		}
		printf(") ");
	}
}
