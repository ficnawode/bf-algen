#pragma once

#include "bf_code_generator/GeneticCodeGenerator.hpp"

namespace bf_code_generator
{
    class Genotype
    {
    public:
        Genotype(const GeneticCodeGenerator* generator) :
            _generator{generator},
            _program{generator->CreateRandomProgram()},
            _score{generator->CalculateFitness(_program)}
        {
        }

        Genotype(const GeneticCodeGenerator* generator, std::string program) :
            _generator{generator},
            _program{program},
            _score{generator->CalculateFitness(program)}
        {
        }

        Genotype(Genotype&& g) = default;
        Genotype& operator=(Genotype&& g) = default;

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
            _score = _generator->CalculateFitness(_program);
        }

        auto operator<=>(const Genotype& g)
        {
            return _score <=> g.GetScore();
        }

        // in cpp20 the compiler generates the other 2-way comparison operators

    private:
        std::string _program;
        double _score;
        const GeneticCodeGenerator* _generator;
    };
}