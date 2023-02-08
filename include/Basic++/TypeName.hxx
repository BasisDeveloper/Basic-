#pragma once
#include <type_traits>
#include <typeinfo>
#if !defined(_MSC_VER)
#include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <cstring>

/* TODO: This can be written out better, it's copy and and
         pasted from stack overflow, and it's not the best. */

template <class FuncParams>
std::string Type_Name()
{
    typedef typename std::remove_reference<FuncParams>::type TR;

    std::unique_ptr<char, void(*)(void*)> own
    (
#ifndef _MSC_VER
        abi::__cxa_demangle(typeid(TR).name(), nullptr,
            nullptr, nullptr),
#else
        nullptr,
#endif
        std::free
    );

    std::string r = own != nullptr ? own.get() : typeid(TR).name();

    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<FuncParams>::value)
        r += "&";
    else if (std::is_rvalue_reference<FuncParams>::value)
        r += "&&";
    return r;
}

template<typename FuncParams>
inline std::string Type_Name(FuncParams object)
{
    return Type_Name<decltype(object)>();
}