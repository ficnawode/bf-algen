#pragma once

#include <random>

namespace util
{
    template <typename FloatLike> FloatLike get_random_real(FloatLike min, FloatLike max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<FloatLike> distr(min, max);
        return distr(gen);
    }

    template <typename IntLike> IntLike get_random_int(IntLike min, IntLike max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<IntLike> distr(min, max);
        return distr(gen);
    }
}