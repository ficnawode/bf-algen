#pragma once

#include <array>
#include <functional>
#include <string>
#include <vector>
#include "bf_code_generator/Genotype.hpp"
#include "bf_interpreter/Interpreter.hpp"

namespace bf_code_generator
{
    class GeneticCodeGenerator
    {
    public:
        GeneticCodeGenerator(const std::string goalOutput);

        void Run();

    private:
        double CalculateFitness(const std::string& program);
        std::string Mutate(std::string program);
        Genotype& SelectParent(const std::string otherParent = "");
        std::array<std::string, 2> Mate(const std::string& parent1, const std::string& parent2);
        bool ProgramExists(const std::string& program);
        double GetTotalPopulationScore() const;

        std::string CreateRandomProgram() const;
        static char GetRandomInstruction();

        const std::string _goalOutput;
        std::vector<Genotype> _population;

        const std::uint16_t _minProgramSize = 10;
        const std::uint16_t _maxProgramSize = 500;
        const std::uint16_t _populationSize = 12;
        const std::uint16_t _numOfCrossovers = 2;
        const double _mutationRate = 0.01;
        const double _errorScore = 1.0;
        const double _lengthPenalty = 0.01;
        const std::uint16_t _displayRate = 6000;

        static constexpr std::array Instructions = {'+', '-', '>', '<', '[', ']', '.'};
    };
}