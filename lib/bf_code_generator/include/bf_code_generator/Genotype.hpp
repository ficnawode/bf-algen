#pragma once

#include <functional>
#include <string>

namespace bf_code_generator
{
    class Genotype
    {
    public:
        Genotype(std::string program, std::function<double(std::string&)> FitnessFunction) :
            _program{program}, _fitnessFunction{FitnessFunction}
        {
            _score = _fitnessFunction(_program);
        }

        Genotype(Genotype&& g) = default;
        Genotype& operator=(Genotype&& g) = default;
        ~Genotype() = default;

        std::string GetProgram() const
        {
            return _program;
        }

        double GetScore() const
        {
            return _score;
        }

        void SetProgram(const std::string program)
        {
            _program = program;
            _score = _fitnessFunction(_program);
        }

        auto operator==(const Genotype& g)
        {
            return _score == g.GetScore();
        }

        auto operator<=>(const Genotype& g)
        {
            return _score <=> g.GetScore();
        }

        // in cpp20 the compiler generates the other 2-way comparison operators

    private:
        std::string _program;
        double _score;
        std::function<double(std::string&)> _fitnessFunction;
    };
}