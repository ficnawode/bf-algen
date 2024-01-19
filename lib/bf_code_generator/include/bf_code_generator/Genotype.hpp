#pragma once

#include <functional>
#include <string>

namespace bf_code_generator
{
    struct Genotype
    {
        std::string program;
        double score;
        auto operator==(const Genotype& g) const
        {
            return score == g.score;
        }

        auto operator<=>(const Genotype& g) const
        {
            return score <=> g.score;
        }
    };
}