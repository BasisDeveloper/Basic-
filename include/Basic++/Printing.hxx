#pragma once
#include "Formatting.hxx"
#include <cstdio>

namespace Basic::Printing
{
    template <size_t N_Characters, Formatting::Formattable... formatables_t>
    void Print(FILE* stream, char const (&msg)[N_Characters], formatables_t... arguments)
    {
        auto msg_formatted = Formatting::Format(msg, arguments...);
        fwrite(msg_formatted.data(), sizeof(char), msg_formatted.length(), stream);
    }

    template <size_t N_Characters, Formatting::Formattable... formatables_t>
    void Println(FILE* stream, char const (&msg)[N_Characters], formatables_t... arguments)
    {
        auto msg_formatted = Formatting::Format(msg, arguments...) + '\n';
        fwrite(msg_formatted.data(), sizeof(char), msg_formatted.length(), stream);
    }

    template <size_t N_Characters, Formatting::Formattable... formatables_t>
    void Print(char const (&msg)[N_Characters], formatables_t... arguments)
    {
        Print(stdout, msg, arguments...);
    }

    template <size_t N_Characters, Formatting::Formattable... formatables_t>
    void Println(char const (&msg)[N_Characters], formatables_t... arguments)
    {
        Println(stdout, msg, arguments...);
    }

    void Print(auto value)
    {
        Print(stdout, "{}", value);
    }

    void Println(auto value)
    {
        Println(stdout, "{}", value);
    }

    void Print(FILE* stream, auto value)
    {
        Print(stream, "{}", value);
    }

    void Println(FILE* stream, auto value)
    {
        Println(stream, "{}", value);
    }
};
