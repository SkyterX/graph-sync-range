#include <test_tools/Message.h>
#include <CppUnitTest.h>
#include <stdarg.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_tools
{
	static const int FormatBufferSize = 1024;

	void Message::Write(const char* message) {
		Logger::WriteMessage(message);
	}

	void Message::WriteF(const char* format, ...) {
		char buffer[FormatBufferSize];
		va_list arg;
		va_start(arg, format);
		_vsnprintf(buffer, sizeof(buffer), format, arg);
		va_end(arg);
		Write(buffer);
	}

	void Message::WriteLine(const char* message) {
		Write(message);
		Write("\r\n");
	}

	void Message::WriteLineF(const char* format, ...) {
		char buffer[FormatBufferSize];
		va_list arg;
		va_start(arg, format);
		_vsnprintf(buffer, sizeof(buffer), format, arg);
		va_end(arg);
		WriteLine(buffer);
	}
}
