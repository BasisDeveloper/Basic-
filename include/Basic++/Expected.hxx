/* Mission: Is to create "zero" cost abstarction for error reporting.
            Using an optimizing compiler the use of these structures
            should result in very VERY low cost, if not zero. */

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

        /* this lambda makes it where you don't have to
           call `status()`, but instead use `status` as a `const` variable. */
        // const char* status = [&]() { return status[0] == '^' ? status + 1 : status;}();

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(T&& _value) : value(std::move(_value)) {}

        Expected(T&& _value, ConstStringReference msg) :value(std::move(_value)), status(msg) {}

        T* operator->() { return &value; }
        T& operator* () { return value; }

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
