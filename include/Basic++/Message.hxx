#pragma once

#include <string>
#include <cstddef>
#include "Formatting.hxx"

namespace Basic
{
    struct Message
    {
        using ConstStringReference = char const(&)[];

        std::string string;

        template<typename ...T>
        constexpr Message(const ConstStringReference& msg, T&& ...args)
        {
            if constexpr (sizeof...(args) > 0)
                string = Basic::Formatting::Format(msg, args...);
        }

        constexpr Message(const ConstStringReference& msg)
        {
            string = Basic::Formatting::Format(msg);
        }
    };
}
