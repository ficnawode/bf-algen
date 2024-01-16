#include "bf_code_generator/GeneticCodeGenerator.hpp"

#include <algorithm>
#include <chrono>
#include "bf_interpreter/Interpreter.hpp"
#include "util/random.hpp"
#include "util/string.hpp"

#include <iostream>

namespace bf_code_generator
{
    using namespace util;

    GeneticCodeGenerator::GeneticCodeGenerator(const std::string goalOutput) :
        _goalOutput{goalOutput},
        CalculateFitness{
            [&](const std::string& program)
            {
                auto interpreter_output = bf_interpreter::Interpreter::Interpret(program);
                if(!interpreter_output.has_value())
                    return _errorScore;
                auto output = interpreter_output.value();

                double score = util::string_distance(output, _goalOutput);
                score += (static_cast<double>(program.length()) * _lengthPenalty);

                static const auto maxScore =
                    static_cast<double>(_goalOutput.length() * UINT8_MAX);
                return maxScore - score;
            }}
    {
        for(int i = 0; i < _populationSize; i++)
        {
            _population.emplace_back(CreateRandomProgram(), CalculateFitness);
        }
    }

    void GeneticCodeGenerator::Run()
    {
        bool keepGoing = false;
        auto startTime = std::chrono::high_resolution_clock::now();

        for(unsigned long generations = 0;; generations++)
        {
            SortPopulation();
            std::string bestProgram = _population[0].GetProgram();

            // crossover
            for(int i = 0; i < _numOfCrossovers; i++)
            {
                auto& parent1 = SelectParent();
                auto& parent2 = SelectParent(parent1.GetProgram());

                auto children = Mate(parent1.GetProgram(), parent2.GetProgram());
                // mutation
                parent1.SetProgram(Mutate(children[0]));
                parent2.SetProgram(Mutate(children[1]));
            }

            // elitism
            if(!ProgramExists(bestProgram))
            {
                auto& worstProgram = _population[_population.size() - 1];
                worstProgram.SetProgram(bestProgram);
            }

            if(!(generations % _displayRate))
            {
                std::cout << "\n\nCurrent best program: " << std::endl;
                std::cout << bestProgram << std::endl;

                auto output = bf_interpreter::Interpreter::Interpret(bestProgram);
                std::cout << "\nOutput: " << output.value_or("Error") << std::endl;
                std::cout << "Value of loss function: " << CalculateFitness(bestProgram)
                          << std::endl;

                if(output == _goalOutput && !keepGoing)
                {
                    std::cout << "\n\a\a\aProgram evolved." << std::endl;

                    auto endTime = std::chrono::high_resolution_clock::now();
                    auto duration =
                        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                    std::cout << "Time taken: " << duration.count()
                              << " milliseconds" << std::endl;
                    std::cout << "Number of generations: " << generations << std::endl;

                    char answer = 'n';
                    std::cout << "Keep evolving for more efficiency? (y/n) ";
                    std::cin >> answer;

                    if(answer != 'y')
                        return;
                    keepGoing = true;
                }
            }

            ++generations;
        }
    }

    char GeneticCodeGenerator::GetRandomInstruction()
    {
        static constexpr auto max = Instructions.size() - 1;
        auto index = get_random<size_t>(0, max);
        return Instructions.at(index);
    }

    std::string GeneticCodeGenerator::CreateRandomProgram() const
    {
        auto program_size = get_random<uint16_t>(_minProgramSize, _maxProgramSize);
        std::string program;

        for(std::uint16_t i = 0; i < program_size; ++i)
        {
            program += GetRandomInstruction();
        }
        return program;
    }

    std::string GeneticCodeGenerator::Mutate(std::string program)
    {
        static const auto AddRandomInstruction = [&](std::string program, std::uint16_t index)
        {
            if((program.length()) < _maxProgramSize)
                program.insert(index, 1, GetRandomInstruction());
            return program;
        };

        static const auto RemoveInstruction = [&](std::string program, std::uint16_t index)
        {
            if(program.length() > _minProgramSize)
                program.erase(index, 1);
            return program;
        };

        static const auto ReplaceWithRandomInstruction =
            [&](std::string program, std::uint16_t index)
        {
            program[index] = GetRandomInstruction();
            return program;
        };

        static const std::array<std::function<std::string(std::string, std::uint16_t)>, 3> mutationFunctions = {
            AddRandomInstruction, RemoveInstruction, ReplaceWithRandomInstruction};
        for(size_t i = 0; i < program.length(); ++i)
        {
            if(_mutationRate >= get_random<float>(0.0, 1.0))
            {
                auto mutation_type =
                    get_random<std::size_t>(0, mutationFunctions.size() - 1);
                program = mutationFunctions.at(mutation_type)(program, i);
            }
        }
        return program;
    }

    double GeneticCodeGenerator::GetTotalPopulationScore() const
    {
        return std::accumulate(
            _population.begin(),
            _population.end(),
            0,
            [](double accumulator, const Genotype& g)
            { return accumulator + g.GetScore(); });
    }

    void GeneticCodeGenerator::SortPopulation()
    {
        std::sort(_population.begin(), _population.end(), std::greater<>());
    }

    Genotype& GeneticCodeGenerator::SelectParent(const std::string other_parent)
    {
        double totalScore = GetTotalPopulationScore();
        auto selectionThreshhold = get_random<double>(0.0, 1.0);
        double previousChance = 0;
        for(auto& g: _population)
        {
            double currentChance = g.GetScore() / totalScore + previousChance;

            if(currentChance > selectionThreshhold && g.GetProgram() != other_parent)
                return g;
            previousChance = currentChance;
        }
        return _population[0];
    }

    std::array<std::string, 2>
        GeneticCodeGenerator::Mate(const std::string& parent1, const std::string& parent2)
    {
        std::string minStr = util::get_shorter_string(parent1, parent2);
        std::string maxStr = util::get_longer_string(parent1, parent2);
        auto crosspoint = get_random<std::size_t>(1, maxStr.length() - 1);

        std::string maxStringContribution = maxStr.substr(crosspoint);
        maxStr.erase(crosspoint);
        if(crosspoint <= minStr.length())
        {
            maxStr += minStr.substr(crosspoint);
            minStr.erase(crosspoint);
        }
        minStr += maxStringContribution;

        return {minStr, maxStr};
    }

    bool GeneticCodeGenerator::ProgramExists(const std::string& program)
    {
        return std::find_if(
                   _population.begin(),
                   _population.end(),
                   [&](const Genotype& g)
                   { return g.GetProgram() == program; }) != _population.end();
    }
}