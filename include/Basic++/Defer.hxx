#pragma once

/* e.g
    int main()
    {
        FILE* file = fopen("somefile", "r");

        defer { fclose(file); };

        // do some work...


    } // at the end of the scope, the `fclose`  will be called.
*/

namespace Defer
{
    template <typename function>
    struct _defer
    {
        const function func;

        constexpr inline _defer(const function&& _func): func(_func) {}

        constexpr inline ~_defer() noexcept
        {
            func();
        }
    };
}

// These TWO CONCAT macros MUST  exist. The indirection makes it __COUNTER__ work.
#define defer_CONCAT_(x,y) x##y

#define defer_CONCAT(x,y) defer_CONCAT_(x,y)

#define defer Defer::_defer defer_CONCAT(_defer_scoped_object, __COUNTER__) = [&]()
