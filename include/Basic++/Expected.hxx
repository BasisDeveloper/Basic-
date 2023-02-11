/* Mission: Is to create "zero" cost abstarction for error reporting. */

#pragma once

#include <utility>

namespace Basic::Expectations
{
    template<typename T = void>
    struct Expected
    {
        using Type = T;
        
        using ConstStringReference = char const(&)[];

        T value;

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(T&& _value):
            value(std::move(_value))
        {}

        Expected(T&& _value, ConstStringReference msg):
            value(std::move(_value)),
            status(msg)
        {}

        T* operator->() { return &value; }
        T& operator* () { return value; }

        constexpr operator bool() const
        {
            return status[0] == '^';
        }
    };

    template<>
    struct Expected<void>
    {
        using Type = void;
        
        using ConstStringReference = char const(&)[];

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(ConstStringReference msg):
            status(msg)
        {}

        constexpr operator bool() const
        {
            return status[0] == '^';
        }
    };
}
