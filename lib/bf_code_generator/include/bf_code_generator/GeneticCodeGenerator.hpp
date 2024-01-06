#pragma once
#include <functional>
#include <span>
#include <string>
#include <vector>
#include "bf_code_generator/fwd.hpp"
#include "bf_interpreter/Interpreter.hpp"

namespace bf_code_generator
{
    class GeneticCodeGenerator;

    class GeneticCodeGenerator
    {
    public:
        GeneticCodeGenerator(std::string goalOutput);
        ~GeneticCodeGenerator() = default;

        void Run();

    private:
        static char GetRandomInstruction();
        std::string CreateRandomProgram() const;
        double CalculateFitness(const std::string& program) const;

        static std::string AddRandomInstruction(std::string program, unsigned index);
        static std::string RemoveInstruction(std::string program, unsigned index);
        static std::string ReplaceWithRandomInstruction(std::string program, unsigned index);

        std::vector<std::function<std::string(const std::string&, unsigned)>> _mutationFunctions = {
            AddRandomInstruction,
            RemoveInstruction,
            ReplaceWithRandomInstruction};

        static std::string Mutate(std::string program);

        double GetTotalPopulationScore();
        void SortPopulation();

        std::string SelectParent(const std::string& other_parent = "");

        void Mate(const std::string& parent1, const std::string& parent2, std::string children[]);

        bool ProgramExists(const std::string& program);

        void ReplaceProgram(const std::string& parent, const std::string& child);

        std::string _goalOutput;
        std::vector<Genotype> _population;

        std::uint16_t _minProgramSize = 10;
        std::uint16_t _maxProgramSize = 500;
        std::uint16_t _populationSize = 10;
        double _mutationRate = 0.01;
        double _errorScore = 1.0;
        double _lengthPenalty = 0.001;
        std::uint16_t _displayRate = 10000;

        // Don't modify this group of constants.

        static constexpr unsigned CHAR_SIZE = 255;
        static constexpr std::array Instructions = {'+', '-', '>', '<', '[', ']', '.'};
        static constexpr unsigned NChildren =
            2; // Number of children two parents create upon reproduction.

        friend class Genotype;
    };
}