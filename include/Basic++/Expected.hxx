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
  using ConstStringReference = char const(&)[];

  static ConstStringReference AOK = "^(AOK)";

  template<typename T>
  struct Expected
  {
  private:
    T _value;
    // const char* const _status_ptr = AOK;
    ConstStringReference _status_ptr = AOK;

  public:
    using Type = T;

    inline ConstStringReference status() const
    {
      return _status_ptr;
    }

    inline const T& value() const
    {
      return _value;
    }

    inline T& value()
    {
      return _value;
    }

    Expected() = default;

    Expected(T&& _value) : _value(std::move(_value)) {}

    Expected(T&& _value, ConstStringReference msg) : _value(std::move(_value)), _status_ptr(msg) {}

    Expected(ConstStringReference msg) : _status_ptr(msg) {}

    T* operator->()
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T>, "
        "or any other kind for that matter.");

      return &_value;
    }

    T& operator* ()
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T>, "
        "or any other kind for that matter.");
      return _value;
    }

    const T& operator* () const
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T>, "
        "or any other kind for that matter.");
      return _value;
    }

    inline auto expect(std::source_location sl = std::source_location::current()) -> T&
    {
        // am I playing with fire here?
      #ifndef NO_EXPECTATIONS
      if (!(this->operator bool())) [[likely]]
      {
        Basic::Expectations::Expect(false, _status_ptr, sl);
        // before we debug break we want to dump anything buffered 
        // we have in stdout so the user can see whatever is in there.
        std::fflush(stdout);

        BASIC_DEBUG_BREAK();

        std::exit(EXIT_FAILURE);
      }
      else
      {
        return _value;
      }
      #else
      return value;
      #endif
    }

    inline auto expect(std::source_location sl = std::source_location::current()) const -> const T&
    {
         // am I playing with fire here?
      #ifndef NO_EXPECTATIONS
      if (!(this->operator bool())) [[likely]]
      {
        Basic::Expectations::Expect(false, _status_ptr, sl);
        // before we debug break we want to dump anything buffered 
        // we have in stdout so the user can see whatever is in there.
        std::fflush(stdout);

        BASIC_DEBUG_BREAK();

        std::exit(EXIT_FAILURE);
      }
      else
      {
        return _value;
      }
      #else
      return value;
      #endif
    }

    // for some silly reason I can't compare the pointers...
    constexpr operator bool() const { return _status_ptr[0] == AOK[0]; }

    ~Expected() {};
  };

  template<typename T>
  struct Expected<T&>
  {
  private:
    ConstStringReference _status = AOK;
    T& _value;
  public:
    using Type = T;

    ConstStringReference status() const { return _status; }

    auto value() -> T& { return _value; }

    Expected() = default;

    Expected(T& _value) : _value(_value) {}

    Expected(T& _value, ConstStringReference msg) : _value(_value), _status(msg) {}

    // yes, this is undefined behavior, yes, I mean to do it.
    // even if we fail we MUST initialize `value` to something, so this is my solution.
    // if the user receives an Expected that has a failure value, then they shouldn't dereference
    // it anyway, if they do, they'll recieved an expection for their carelessness.
    Expected(ConstStringReference msg) : _value(reinterpret_cast<T&>(*(T*)-0)), _status(msg) {}

    T& operator* ()
    {
      EXPECT((this->operator bool()) == true,
        "you mustn't dereference an invalid Expected<T&>, "
        "or any other kind for that matter.");

      return _value;
    }

    inline auto expect(std::source_location sl = std::source_location::current()) -> T&
    {
        // am I playing with fire here?
      #ifndef NO_EXPECTATIONS
      if (!(this->operator bool())) [[likely]]
      {
        Basic::Expectations::Expect(false, _status, sl);
        // before we debug break we want to dump anything buffered 
        // we have in stdout so the user can see whatever is in there.
        std::fflush(stdout);

        BASIC_DEBUG_BREAK();

        std::exit(EXIT_FAILURE);
      }
      else
      {
        return _value;
      }
      #else
      return value;
      #endif
    }

    inline auto expect(std::source_location sl = std::source_location::current()) const -> const T&
    {
      // am I playing with fire here?
      #ifndef NO_EXPECTATIONS
      if (!(this->operator bool())) [[likely]]
      {
        Basic::Expectations::Expect(false, _status, sl);
        // before we debug break we want to dump anything buffered 
        // we have in stdout so the user can see whatever is in there.
        std::fflush(stdout);

        BASIC_DEBUG_BREAK();

        std::exit(EXIT_FAILURE);
      }
      else
      {
        return _value;
      }
      #else
      return value;
      #endif
    }

    constexpr operator bool() const { return _status[0] == AOK[0]; }
  };

  /*
   this structures enables return value like:
      `return "error encountred, couldn't do X, sorry.";`
   (I'd prefer if you'd just use the Expected<bool> overload).
  */
  template<>
  struct Expected<void>
  {
  private:
    ConstStringReference _status = AOK;
  public:
    using Type = void;

    ConstStringReference status() const { return _status; }

    Expected() = default;

    Expected(ConstStringReference msg) : _status(msg) {}

    constexpr operator bool() const { return _status[0] == AOK[0]; }
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
  private:
    const bool _value = false;
    ConstStringReference _status = AOK;
  public:
    using Type = bool;

    using ConstStringReference = char const(&)[];

    const bool& value() { return _value; }

    ConstStringReference status() const { return _status; }

    Expected() = default;

    Expected(ConstStringReference msg) : _status(msg) {}

    Expected(bool&& _value) : _value(_value) {}

    Expected(bool&& _value, ConstStringReference msg) : _value(_value), _status(msg) {}

    const bool* operator->() { return &value(); }
    const bool& operator* () { return value(); }

    constexpr operator bool() const { return +_status == +AOK; }
  };

  using Err = Expected<bool>;
}