/* Mission: Is to create "zero" cost abstarction for error reporting.
            Using an optimizing compiler the use of these structures
            should result in very VERY low cost, if not zero. */

#pragma once

#include <utility>

namespace Basic::Expectations
{
    template<typename T = void>
    struct Expected;
} // FIXME: this is terrible, I hate this.
// the Basic++/Expectations.hxx header depends on us, be we also depend on them... gosh...

#include "Basic++/Expectations.hxx"

namespace Basic::Expectations
{
    template<typename T>
    struct Expected
    {
        using Type = T;

        using ConstStringReference = char const(&)[];

        T value;

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(T&& _value) : value(std::move(_value)) {}

        Expected(T&& _value, ConstStringReference msg) :value(std::move(_value)), status(msg) {}

        Expected(ConstStringReference msg) : value(T{}), status(msg) {}

        T* operator->() { return &value; } // FIXME: this prohbits refernces types from being in an expected.
        T& operator* () { return value; }

        auto expect(std::source_location sl = std::source_location::current()) -> T&
        {
            if (Basic::Expectations::Expect((this->operator bool()), status, sl))
                return value;

            BASIC_DEBUG_BREAK();
            std::exit(EXIT_FAILURE);
        }

        constexpr operator bool() const { return status[0] == '^'; }
    };

    /*
     this structures enables return value like:
        `return "error encountred, couldn't do X, sorry.";`
     (I'd prefer if you'd just use the Expected<bool> overload).
    */
    template<>
    struct Expected<void>
    {
        using Type = void;

        using ConstStringReference = char const(&)[];

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(ConstStringReference msg) : status(msg) {}

        constexpr operator bool() const { return status[0] == '^'; }
    };

    /*
     this structures enables return value like:
        `return false;`
        or:
        `return { false, "this is an error message..." };
        or even:
        `return { true, "this is a success message!" };
    */

    template<>
    struct Expected<bool>
    {
        using Type = bool;

        using ConstStringReference = char const(&)[];

        const bool value = false;

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(ConstStringReference msg) :status(msg) {}

        Expected(bool&& _value) : value(_value) {}

        Expected(bool&& _value, ConstStringReference msg) : value(_value), status(msg) {}

        const bool* operator->() { return &value; }
        const bool& operator* () { return value; }

        constexpr operator bool() const { return status[0] == '^' or value == true; }
    };

    using Err = Expected<bool>;
}
