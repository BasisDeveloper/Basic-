#pragma once

#include <utility>
#include <cstddef>
#include <type_traits>

// Function Complete Message?
// Function Complete Message?
// Routine Completion Status
// Completion Status ☑️

namespace Basic::Expectations
{
    template<typename T>
    struct Expected
    {
        using Type = T;

        T* _value;

        // T& value = *_value;

        using ConstStringReference = char const(&)[];

        ConstStringReference completion_status;

        constexpr Expected(const Expected<T>& other) = delete;

        T& operator->() const { return *_value; }

        T& operator* () const { return *_value; }

        constexpr Expected(T& _val):
            _value(&_val),
            completion_status("^(empty)") {}

        constexpr Expected(T* _val) :
            _value(std::move(_val)),
            completion_status("^(empty)") {}

        constexpr Expected(T&& _val) :
            _value(&_val),
            completion_status("^(empty)") {}
        
        constexpr Expected(T& _val, ConstStringReference csr) requires (not std::is_pointer_v<T>) :
            _value(&_val),
            completion_status(csr) {}

        constexpr Expected(T&& _val, ConstStringReference csr) requires (not std::is_pointer_v<T>) :
            _value(&_val), completion_status{ csr } {}

        constexpr Expected(T* _val, ConstStringReference csr) requires (std::is_pointer_v<T>) :
            _value(std::move(_val)), completion_status{ csr } {}

        operator bool() const
        {
            return completion_status[0] == '^';
        }
    };
}
