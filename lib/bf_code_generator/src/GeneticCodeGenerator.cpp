#include "bf_code_generator/GeneticCodeGenerator.hpp"
#include <algorithm>
#include "bf_code_generator/Genotype.hpp"
#include "bf_interpreter/Interpreter.hpp"
#include "util/util.hpp"

namespace bf_code_generator
{
    using namespace util;

    GeneticCodeGenerator::GeneticCodeGenerator(std::string goalOutput)
    {
        for(int i = 0; i < _populationSize; i++)
        {
            _population.push_back(Genotype(this));
        }
    }

    void GeneticCodeGenerator::Run()
    {
    }

    char GeneticCodeGenerator::GetRandomInstruction()
    {
        auto min = 0;
        auto max = Instructions.size();
        auto index = get_random_int<size_t>(min, max);
        return Instructions[index];
    }

    std::string GeneticCodeGenerator::CreateRandomProgram() const
    {
        std::string program;
        uint16_t program_size = get_random_int<uint16_t>(_minProgramSize, _maxProgramSize);

        for(unsigned i = 0; i < program_size; ++i)
        {
            program += GetRandomInstruction();
        }
        return program;
    }

    double GeneticCodeGenerator::CalculateFitness(const std::string& program) const
    {
        // The score of the worst program possible (Besides erroneous program, and not taking into account program length).
        double max_score = _goalOutput.size() * CHAR_SIZE;
        double score = 0;
        double final_score;

        using namespace bf_interpreter;
        auto interpreter_output = Interpreter::Interpret(program);

        // Impose a very large penalty for error programs, but still allow them a chance at reproduction for genetic variation.
        if(!interpreter_output.has_value())
            return _errorScore;

        auto output = interpreter_output.value();
        /* We need to know whether the goal output or the program's output is larger
           because that's how many iterations of the next loop need to be done. */
        std::string min_str = (output.length() < _goalOutput.length()) ? output : _goalOutput;
        std::string max_str = (output.length() >= _goalOutput.length()) ? output : _goalOutput;

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
        final_score = max_score - score;

        return final_score;
    }

    std::string GeneticCodeGenerator::AddRandomInstruction(std::string program, unsigned index)
    {
    }
    std::string GeneticCodeGenerator::RemoveInstruction(std::string program, unsigned index)
    {
    }
    std::string GeneticCodeGenerator::ReplaceWithRandomInstruction(std::string program, unsigned index)
    {
    }
    std::string GeneticCodeGenerator::Mutate(std::string program)
    {
    }

    double GeneticCodeGenerator::GetTotalPopulationScore()
    {
        double temp = 0;
        for(auto& g: _population)
        {
            temp += g.GetScore();
        }
        return temp;
    }

    void GeneticCodeGenerator::SortPopulation()
    {
        std::sort(_population.begin(), _population.end());
    }

    std::string GeneticCodeGenerator::SelectParent(const std::string& other_parent)
    {
        return "";
    }

    void GeneticCodeGenerator::Mate(const std::string& parent1, const std::string& parent2, std::string children[])
    {
    }

    bool GeneticCodeGenerator::ProgramExists(const std::string& program)
    {
        for(auto& g: _population)
        {
            if(g.GetProgram() == program)
                return true;
        }
        return false;
    }

    void GeneticCodeGenerator::ReplaceProgram(const std::string& parent, const std::string& child)
    {
        for(auto& g: _population)
        {
            if(g.GetProgram() == parent)
            {
                g.SetProgram(child);
                break;
            }
        }
    }
}