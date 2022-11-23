#pragma once

#include <random>

namespace Math
{
    auto Random_Numeric_Range(auto from, auto to)
    {
        std::random_device rand_device;
        std::default_random_engine rand_engine(rand_device());

        std::uniform_real_distribution<decltype(from)> uniform_dist(from, to);

        return uniform_dist(rand_engine);
    }
};