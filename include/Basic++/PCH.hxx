// I'm going to include some in-project files that I think/know will pretty much never change.
#include "Expectations.hxx"
#include "Common.hxx"
#include "Defer.hxx"
#include "TypeName.hxx"
#include "Mathematics.hxx"

//TODO: Check if we're actually on Windows.
#if defined(_WIN32)
#include <Windows.h> 
#endif