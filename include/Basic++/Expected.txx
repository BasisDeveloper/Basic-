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

        T* operator->() { return &value; }
        T& operator* () { return value; }

        inline auto expect(std::source_location sl = std::source_location::current()) -> T&
        {
            // am I playing with fire here?
            #ifndef NO_EXPECTATIONS
            if (Basic::Expectations::Expect((this->operator bool()), status, sl)) [[likely]]
                return value;
            BASIC_DEBUG_BREAK();
            std::exit(EXIT_FAILURE);
            std::unreachable();
            #else
            
            return value;
            #endif
        }

        constexpr operator bool() const { return status[0] == '^'; }
    };

    template<typename T>
    struct Expected<T&>
    {
        using Type = T;

        using ConstStringReference = char const(&)[];

        T value;

        ConstStringReference status = "^(AOK)";

        Expected() = default;

        Expected(T& _value) : value(std::move(_value)) {}

        Expected(T& _value, ConstStringReference msg) :value(std::move(_value)), status(msg) {}

        Expected(ConstStringReference msg) : value(T{}), status(msg) {}

        T& operator* () { return value; }

        inline auto expect(std::source_location sl = std::source_location::current()) -> T&
        {
            // am I playing with fire here?
            #ifndef NO_EXPECTATIONS
            if (Basic::Expectations::Expect((this->operator bool()), status, sl)) [[likely]]
                return value;
            BASIC_DEBUG_BREAK();
            std::exit(EXIT_FAILURE);
            std::unreachable();
            #else
            return value;
            #endif
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
