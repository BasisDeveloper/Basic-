/* Mission: Is to create "zero" cost abstarction for error reporting.
            Using an optimizing compiler the use of these structures
            should result in very VERY low cost, if not zero. */

#pragma once

#include <utility>
#include <source_location>
#include <cstdlib>

namespace Basic::Expectations {
  template<typename T>
  struct Expected;
}

#include "Basic++/Expectations.hxx"

namespace Basic::Expectations
{
  template<typename T>
  struct Expected
  {
    using Type = T;

    using ConstStringReference = char const(&)[];

    constexpr static const char AOK[7] = "^(AOK)";
    constexpr static const char NOK[7] = "!(NOK)";

    union
    {
      T value{};
      const char* status;
    };

    Expected() = default;

    Expected(T&& _value) : value(std::move(_value))
    {
      printf("%d", _value);
    }

    // Expected(T&& _value, ConstStringReference msg) :value(std::move(_value)), status(msg) {}

    Expected(ConstStringReference msg) : status(msg) {}

    T* operator->()
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T>, "
        "or any other kind for that matter.");

      return &value;
    }

    T& operator* ()
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T>, "
        "or any other kind for that matter.");
      return value;
    }

    inline auto expect(std::source_location sl = std::source_location::current()) -> T&
    {
        // am I playing with fire here?
      #ifndef NO_EXPECTATIONS
      if (!(this->operator bool()))
      { // this if condition is not by accident we must do it first, otherwise the
        // `Expect` function below because will pack up the status string, 
        //  run Basic::Formatting::Format on it which may call a crash
        //  because the status string may point to non-sense. This is 
        //  technically a problem.
        if (Basic::Expectations::Expect((this->operator bool()), status, sl)) [[likely]]
        {
          return value;
        }
        else
        {
          // before we debug break we want to dump anything buffered 
          // we have in stdout so the user can see whatever is in there.
          std::fflush(stdout);

          BASIC_DEBUG_BREAK();

          std::exit(EXIT_FAILURE);
        }
      }
      else
      {
        return value;
      }
      #else
      return value;
      #endif
    }

    constexpr operator bool() const { return (status != AOK); }
  };

  template<typename T>
  struct Expected<T&>
  {
  public:
    using Type = T;

    using ConstStringReference = char const(&)[];

    T& value;

    ConstStringReference status = "^(AOK)";

    Expected() = default;

    Expected(T& _value) : value(_value) {}

    Expected(T& _value, ConstStringReference msg) : value(_value), status(msg) {}

    // yes, this is undefined behavior, yes, I mean to do it.
    // even if we fail we MUST initialize `value` to something, so this is my solution.
    // if the user receives an Expected that has a failure value, then they shouldn't dereference
    // it anyway, if they do, they'll recieved an expection for their carelessness.
    Expected(ConstStringReference msg) : value(reinterpret_cast<T&>(*(T*)-0)), status(msg) {}

    T& operator* ()
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T&>, "
        "or any other kind for that matter.");

      return value;
    }

    inline auto expect(std::source_location sl = std::source_location::current()) -> T&
    {
        // am I playing with fire here?
      #ifndef NO_EXPECTATIONS
      if (Basic::Expectations::Expect((this->operator bool()), status, sl)) [[likely]]
      {
        return value;
      }
      else
      {
        // before we debug break we want to dump anything buffered 
        // we have in stdout so the user can see whatever is in there.
        std::fflush(stdout);

        BASIC_DEBUG_BREAK();

        std::exit(EXIT_FAILURE);
      }
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

    constexpr static const char AOK[7] = "^(AOK)";
    constexpr static const char NOK[7] = "!(NOK)";

    const bool value = false;

    ConstStringReference status = AOK;

    Expected() = default;

    Expected(ConstStringReference msg) :status(msg) {}

    Expected(bool&& _value) : value(_value), status((_value == true ? AOK : NOK)) {}

    Expected(bool&& _value, ConstStringReference msg) : value(_value), status(msg) {}

    const bool* operator->() { return &value; }
    const bool& operator* () { return value; }

    constexpr operator bool() const { return status[0] == '^' or value == true; }
  };

  using Err = Expected<bool>;
}