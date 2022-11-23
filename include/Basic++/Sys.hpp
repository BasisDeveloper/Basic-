#pragma once
#include <string>

namespace Sys
{
    using exit_code_t = std::size_t;
    constexpr exit_code_t INVALID_EXIT_CODE = (size_t)-1;

    struct ShellExecuteResult
    {
        std::string out;
        std::string err;
        std::size_t exit_code = INVALID_EXIT_CODE;
    };

    ShellExecuteResult Shell_Execute_Write_Then_Read(
        const std::string& command,
        const std::string_view& msg,
        bool wait_for_process_exit_before_read = false);
};