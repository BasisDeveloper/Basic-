#include "Basic++/Expectations.hxx"
using namespace Basic::Expectations;

#include "Basic++/Expected.hxx"

#include "Basic++/Printing.hxx"
using namespace Basic::Printing;

auto An_Expected_Of_Int(int v) -> Expected<int>
{
    if (v > 5)
        return { __FUNCTION__ };
    return 5;
}

auto An_Expected_Of_Const_Int_Reference(const int& v) -> Expected<decltype(v)>
{
    if (v > 5)
        return { __FUNCTION__ };
    return v;
}

auto An_Expected_Of_Newed_Int_Reference(int v) -> Expected<int&>
{
    if (v > 5)
        return { __FUNCTION__ };

    int* new_int = new int{ v };

    return *new_int;
}

int main()
{
    Println("Hello, From Sandbox.exe!");

    Println(An_Expected_Of_Int(5).expect());

    int integer_to_referenced = 5;

    Println(An_Expected_Of_Const_Int_Reference(integer_to_referenced).expect());

    Println(An_Expected_Of_Newed_Int_Reference(5).expect());

    integer_to_referenced = 10;

    *An_Expected_Of_Const_Int_Reference(integer_to_referenced);

    Println(An_Expected_Of_Int(6).expect());

    Println("Bye, bye, from Sandbox.exe!");
}