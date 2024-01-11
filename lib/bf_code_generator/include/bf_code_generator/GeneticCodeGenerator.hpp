#pragma once

#include <array>
#include <functional>
#include <span>
#include <string>
#include <vector>
#include "bf_code_generator/Genotype.hpp"
#include "bf_code_generator/fwd.hpp"
#include "bf_interpreter/Interpreter.hpp"

namespace bf_code_generator
{
    class GeneticCodeGenerator
    {
    public:
        GeneticCodeGenerator(const std::string goalOutput);

        void Run();

    private:
        static char GetRandomInstruction();
        std::string CreateRandomProgram() const;

        std::function<double(std::string&)> CalculateFitness = [&](const std::string& program)
        {
            // The score of the worst program possible (Besides erroneous program, and not taking into account program length).
            double max_score = _goalOutput.size() * CHAR_SIZE;
            double score = 0;

            using namespace bf_interpreter;
            auto interpreter_output = Interpreter::Interpret(program);
            if(!interpreter_output.has_value())
                return _errorScore;
            auto output = interpreter_output.value();

            std::string min_str =
                (output.length() < _goalOutput.length()) ? output : _goalOutput;
            std::string max_str =
                (output.length() >= _goalOutput.length()) ? output : _goalOutput;

            for(size_t i = 0; i < max_str.length(); ++i)
            {
                unsigned output_char;
                if(i < min_str.length())
                    output_char = min_str[i];
                else
                    output_char = max_str[i] + CHAR_SIZE;

                score += abs(output_char - max_str[i]);
            }

            score += (program.length() * _lengthPenalty);
            double final_score = max_score - score;

            return final_score;
        };

        std::function<std::string(const std::string, unsigned)> AddRandomInstruction =
            [&](std::string program, unsigned index)
        {
            std::string instr(1, GetRandomInstruction());
            if((program.length()) < _maxProgramSize)
                program.insert(index, instr);
            return program;
        };

        std::function<std::string(const std::string, unsigned)> RemoveInstruction =
            [&](std::string program, unsigned index)
        {
            if((program.length() - 1) > _minProgramSize)
                program.erase(index, 1);
            return program;
        };

        std::function<std::string(const std::string, unsigned)> ReplaceWithRandomInstruction =
            [&](std::string program, unsigned index)
        {
            program[index] = GetRandomInstruction();
            return program;
        };

        std::array<std::function<std::string(const std::string, unsigned)>, 3> _mutationFunctions = {
            AddRandomInstruction,
            RemoveInstruction,
            ReplaceWithRandomInstruction};

        std::string Mutate(std::string program);

        double GetTotalPopulationScore();
        void SortPopulation();

        Genotype& SelectParent(const std::string other_parent = "");

        std::array<std::string, 2> Mate(const std::string& parent1, const std::string& parent2);

        bool ProgramExists(const std::string& program);

        void ReplaceProgram(const std::string& parent, const std::string& child);

        const std::string _goalOutput;
        std::vector<Genotype> _population;

        std::uint16_t _minProgramSize = 10;
        std::uint16_t _maxProgramSize = 500;
        std::uint16_t _populationSize = 10;
        std::uint16_t _numOfCrossovers = 3;
        double _mutationRate = 0.01;
        double _errorScore = 2.0;
        double _lengthPenalty = 0.01;
        std::uint16_t _displayRate = 10000;

        // Don't modify this group of constants.

        static constexpr unsigned CHAR_SIZE = 255;
        static constexpr std::array Instructions = {'+', '-', '>', '<', '[', ']', '.'};
    };
}