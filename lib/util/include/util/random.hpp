#pragma once

#include <random>
#include <type_traits>

namespace util
{
    template <typename Number> Number get_random(Number min, Number max)
    {
        static_assert(std::is_integral_v<Number> || std::is_floating_point_v<Number>);

        static constexpr std::random_device rd;
        static std::mt19937 gen(rd());
        if constexpr(std::is_integral_v<Number>)
        {
            std::uniform_int_distribution<Number> distr(min, max);
            return distr(gen);
        }
        else if(std::is_floating_point_v<Number>)
        {
            std::uniform_real_distribution<Number> distr(min, max);
            return distr(gen);
        }
    }
}
