#pragma once

#include <source_location>
#include <filesystem>

#include "Basic++/Printing.hxx"
#include "Basic++/Message.hxx"
#include "Basic++/Expected.hxx"

// #include "Expected.hxx"

// TODO: This should be here, but it's easy to remove once it REALLY doesn't need to be here... so.
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <debugapi.h> // For DebugBreak()

// Later on, we should use a modified version of https://github.com/scottt/debugbreak.
#define BASIC_DEBUG_BREAK() DebugBreak();

namespace Basic
{
    namespace Expectations
    {
        #if !defined(BASIC_PCH)
        static
            #endif
            bool Expect(
                bool condition,
                const Message&& msg,
                const std::source_location& source_location = std::source_location::current())
        {
            if (!condition) [[unlikely]]
            {
                Basic::Printing::Print("expectation not satisfied : {}", msg.string.data());

                const auto file_name =
                    std::filesystem::path(source_location.file_name()).filename().string();

                const char* function_name = source_location.function_name();

                Basic::Printing::Println(" ~ file:'{}', function:'{}', line:'{}:{}'",
                    file_name.data(), function_name, source_location.line(), source_location.column());
            }

            return condition;
        }

        #if !defined(BASIC_PCH)
        static
            #endif
            template<typename T>
        bool Expect(
            Expected<T> expected,
            const std::source_location& source_location = std::source_location::current())
        {
            return Expect(expected == true, Message{ expected.completion_status }, source_location);
        }
    }
}

#define EXPECT(_cond_, _msg_, ...)                                                                         \
        if (!Basic::Expectations::Expect((_cond_), {_msg_, __VA_ARGS__}, std::source_location::current())) \
		{																					               \
			BASIC_DEBUG_BREAK(); std::exit(EXIT_FAILURE);									               \
		}; 																					               

#define EEXPECT(_expected_)                                                         \
        if (!Basic::Expectations::Expect<decltype(_expected_)::Type>(_expected_,    \
                                 std::source_location::current()))                  \
        {                                                                           \
            BASIC_DEBUG_BREAK(); std::exit(EXIT_FAILURE);                           \
        }                                                                           \
