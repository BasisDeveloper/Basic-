// #define NO_EXPECTATIONS

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

auto An_Expected_Of_Int_Reference(int& v) -> Expected<decltype(v)>
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

auto An_Expected_Of_String(std::string v) -> Expected<std::string>
{
    if (v == "fail")
        return { __FUNCTION__ };

    return v;
}

auto An_Expected_Of_String_Reference(std::string& v) -> Expected<std::string&>
{
    if (v == "fail")
        return { __FUNCTION__ };

    return v;
}

class NTT
{
public:
    int value = {};
    bool fail = false;

    NTT(const NTT& other)
    {
        this->value = other.value;
        this->fail = other.fail;
        Println("NTT move constructed!");
    }

    NTT(const NTT&& other)
    {
        this->value = other.value;
        this->fail = other.fail;
        Println("NTT move constructed!");
    }

    NTT()
    {
        Println("NTT constructed!");
    }
    ~NTT()
    {
        Println("NTT destructed!");
    }
}; static_assert(!std::is_trivial_v<NTT>);

auto An_Expected_Of_A_Non_Trival_Type(NTT n) -> Expected<NTT>
{
    if (n.fail)
        return "failure requested!";

    return n;
}

auto An_Expected_Of_A_Non_Trival_Type_Ref(NTT& n) -> Expected<NTT&>
{
    if (n.fail)
        return "failure requested!";

    return n;
}

int main()
{
    #if 0
    Println("{}", sizeof(Expected<int>));

    Println("Hello, From Sandbox.exe!");

    Println(An_Expected_Of_Int(5).expect());

    int integer_to_referenced = 5;

    Println(An_Expected_Of_Const_Int_Reference(integer_to_referenced).expect());

    Println(An_Expected_Of_Newed_Int_Reference(5).expect());

    An_Expected_Of_String("Pass!").expect();

    std::string str = "pass!";

    auto str_ref = An_Expected_Of_String(str).expect();

    assert((str_ref == "pass!"));

    integer_to_referenced = 10;

    Println("!!! Failing On Purpose !!!");
    auto& invalid_ref = *An_Expected_Of_Int_Reference(integer_to_referenced);

    invalid_ref = 4;

    Println(An_Expected_Of_Int(6).expect());
    #endif

    // copy:
    Println("copy:"); {
        // look at the logs, you can see why using Expected<T&> is better
        NTT ntt;
        ntt.value = 69;
        assert(An_Expected_Of_A_Non_Trival_Type(ntt).expect().value == 69);
    }
    // ref:
    Println("ref:"); {
        NTT ntt;
        ntt.value = 69;
        assert(An_Expected_Of_A_Non_Trival_Type_Ref(ntt).expect().value == 69);
    }
    // fail on purpose:
    {
        NTT ntt;
        ntt.fail = true;
        auto expected_ntt = An_Expected_Of_A_Non_Trival_Type(ntt);

        if (!expected_ntt)
            Println(expected_ntt.status());
    }

    Println("Bye, bye, from Sandbox.exe!");
}