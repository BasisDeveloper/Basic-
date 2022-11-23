#pragma once

#include <source_location>
#include <filesystem>

#include "Print.hxx"
#include "Message.hxx"

// TODO: This should be here, but it's easy to remove once it REALLY doesn't need to be here... so.
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <debugapi.h> // For DebugBreak()

// Later on, we should use a modified version of https://github.com/scottt/debugbreak.
#define BASIC_DEBUG_BREAK() DebugBreak();

namespace Basic
{
	#if defined(BASIC_PCH)
	static
	#endif
		bool Expect(
			bool condition,
			const Message&& msg,
			const std::source_location& source_location = std::source_location::current())
	{
		if (!condition) [[unlikely]]
		{
			Basic::Logging::Print("expectation not satisfied : {}", msg.string.data());

			const auto file_name =
				std::filesystem::path(source_location.file_name()).filename().string();

			const char* function_name = source_location.function_name();

			Basic::Logging::Println(" ~ file:'{}', function:'{}', line:'{}:{}'",
				file_name.data(), function_name, source_location.line(), source_location.column());
		}

		return condition;
	}
}

#define EXPECT(_cond_, _msg_, ...) \
        if (!Basic::Expect((_cond_), {_msg_, __VA_ARGS__}, std::source_location::current())) \
		{																					 \
			BASIC_DEBUG_BREAK(); std::exit(EXIT_FAILURE);									 \
		}; 																					 \
