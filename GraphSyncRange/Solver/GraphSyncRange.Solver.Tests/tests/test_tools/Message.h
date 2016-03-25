#pragma once

namespace test_tools
{
	class Message
	{
	public:
		static void Write(const char* message);
		static void WriteF(const char* format, ...);
		static void WriteLine(const char* message);
		static void WriteLineF(const char* format, ...);
	};
}