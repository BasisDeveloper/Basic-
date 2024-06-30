#include "Basic++/Printing.hxx"
using namespace Basic::Printing;

#include "Basic++/Expected.hxx"
using namespace Basic::Expectations;

auto Dummy(int v) -> Expected<int>
{
    if (v > 5)
        return { "bruh, v too big, bruh." };
    return 5;
}

int main()
{
    Println("Hello, From Sandbox.exe!");

    Println(Dummy(5).expect());

    Println(Dummy(6).expect());

    Println("Bye, bye, from Sandbox.exe!");
}