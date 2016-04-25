#pragma once
#include <cinttypes>
#include <cstdio>
#include <graph/Graph.h>

namespace graph
{
	class Graph6Reader {
		using NumberType = int64_t;
		static const int BufferSize = 2048;
		char buffer[BufferSize];
		FILE* inputFile;

	public:
		using ValueType = Graph;

		ValueType Current;

		Graph6Reader(const char* filename);

		Graph6Reader(FILE* inputFile = stdin);

		bool MoveNext();

	private:
		NumberType ParseNumber(int& pos);

		void ParseGraph();
	};
}
