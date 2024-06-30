/* Mission: Is to create "zero" cost abstarction for error reporting.
            Using an optimizing compiler the use of these structures
            should result in very VERY low cost, if not zero. */

#pragma once

#include <utility>

namespace Basic::Expectations
{
    template<typename T = void>
    struct Expected;
} // FIXME: this is terrible, I hate this.
  // the Basic++/Expectations.hxx header depends on us, be we also depend on them... gosh...

#ifndef EXPECTED_HXX
#define EXPECTED_HXX

#include "Expected.txx"

#endif
