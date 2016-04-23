#include <graph/Graph6Reader.h>
#include <graph/Graph.h>
#include <cassert>
using namespace std;

namespace graph
{
	static const char MinChar = 63;
	static const char MaxChar = 126;
	static const int BitsPerChar = 6;

	struct BitVectorReader {
		char* buffer;
		int currentBit;

		BitVectorReader(char* buffer)
			:buffer(buffer) {
			currentBit = 1 << BitsPerChar;
		}

		bool Next() {
			currentBit >>= 1;
			if (currentBit == 0) {
				++buffer;
				currentBit = 1 << (BitsPerChar - 1);
			}
			return ((*buffer - MinChar) & currentBit) != 0;
		}
	};


	Graph6Reader::Graph6Reader(const char* filename) {
		if (fopen_s(&inputFile, filename, "rt") != 0) {
			fprintf(stderr, "Can't open file : %s\n", filename);
			exit(EXIT_FAILURE);
		}
		MoveNext();
	}

	Graph6Reader::Graph6Reader(FILE* inputFile)
		:inputFile(inputFile), Current(0) {
		MoveNext();
	}

	bool Graph6Reader::MoveNext() {
		if (fscanf_s(inputFile, "%s", &buffer, BufferSize) == EOF) {
			Current.edges.clear();
			return false;
		}
		ParseGraph();
		return true;
	}

	Graph6Reader::NumberType Graph6Reader::ParseNumber(int& pos) {
		if (buffer[pos] != MaxChar) {
			return buffer[pos++] - MinChar;
		}
		NumberType number = 0;
		if (buffer[pos + 1] != MaxChar) {
			for (int i = 0; i < 3; ++i) {
				number = (number << 6) + (buffer[pos + 1 + i] - MinChar);
			}
			pos += 4;
		}
		else {
			for (int i = 0; i < 6; ++i) {
				number = (number << 6) + (buffer[pos + 1 + i] - MinChar);
			}
			pos += 5;
		}
		return number;
	}

	void Graph6Reader::ParseGraph() {
		assert(buffer[0] == '&');
		int pos = 1;
		auto n = ParseNumber(pos);
		Current.edges.resize(n);
		for (int v = 0; v < n; ++v) {
			Current.edges[v].clear();
		}

		BitVectorReader reader(buffer + pos);
		for (int from = 0; from < n; ++from) {
			for (int to = 0; to < n; ++to) {
				if (reader.Next())
					Current.AddEdge(from, to);
			}
		}
	}
}
