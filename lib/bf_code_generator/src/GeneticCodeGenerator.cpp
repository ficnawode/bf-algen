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
        _goalOutput{goalOutput}
    {
        for(int i = 0; i < _populationSize; i++)
        {
            _population.emplace_back(CreateRandomProgram(), 0);
        }
    }

    void GeneticCodeGenerator::Run()
    {
        bool keepGoing = false;
        const auto startTime = std::chrono::high_resolution_clock::now();

        for(unsigned long generations = 0;; generations++)
        {
            // Score the population
            for(auto& g: _population)
            {
                g.score = CalculateFitness(g.program);
            }

            const auto [worstGenotype, bestGenotype] =
                std::minmax_element(_population.begin(), _population.end());
            auto bestProgram = bestGenotype->program;

            // crossover
            for(int i = 0; i < _numOfCrossovers; i++)
            {
                auto& parent1 = SelectParent();
                auto& parent2 = SelectParent(parent1.program);

                auto children = Mate(parent1.program, parent2.program);
                // mutation
                parent1.program = Mutate(children[0]);
                parent2.program = Mutate(children[1]);
            }

            // elitism
            if(!ProgramExists(bestProgram))
            {
                worstGenotype->program = bestProgram;
            }

            if(!(generations % _displayRate))
            {
                std::cout << "\n\nCurrent best program: " << '\n';
                std::cout << bestProgram << '\n';

                const auto output = bf_interpreter::Interpreter::Interpret(bestProgram);
                std::cout << "\nOutput: " << output.value_or("Error") << '\n';
                std::cout << "Value of loss function: " << CalculateFitness(bestProgram)
                          << '\n';

                if(output == _goalOutput && !keepGoing)
                {
                    std::cout << "\n\a\a\aProgram evolved." << '\n';

                    const auto endTime = std::chrono::high_resolution_clock::now();
                    const auto duration =
                        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                    std::cout << "Time taken: " << duration.count()
                              << " milliseconds" << '\n';
                    std::cout << "Number of generations: " << generations << '\n';

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
        auto index = get_random<std::size_t>(0, max);
        return Instructions.at(index);
    }

    std::string GeneticCodeGenerator::CreateRandomProgram() const
    {
        const auto program_size =
            get_random<std::uint16_t>(_minProgramSize, _maxProgramSize);
        std::string program;

        for(std::uint16_t i = 0; i < program_size; ++i)
        {
            program += GetRandomInstruction();
        }
        return program;
    }

    double GeneticCodeGenerator::CalculateFitness(const std::string& program)
    {
        const auto interpreter_output = bf_interpreter::Interpreter::Interpret(program);
        if(!interpreter_output.has_value())
            return _errorScore;
        const auto output = interpreter_output.value();

        double score = util::string_distance(output, _goalOutput);
        score += (static_cast<double>(program.length()) * _lengthPenalty);

        static const auto maxScore = static_cast<double>(_goalOutput.length() * UINT8_MAX);
        return maxScore - score;
    }

    std::string GeneticCodeGenerator::Mutate(std::string program)
    {
        static const auto AddRandomInstruction = [&](std::string& program, std::uint16_t index)
        {
            if(program.length() < _maxProgramSize)
                program.insert(index, 1, GetRandomInstruction());
        };

        static const auto RemoveInstruction = [&](std::string& program, std::uint16_t index)
        {
            if(program.length() > _minProgramSize)
                program.erase(index, 1);
        };

        static const auto ReplaceWithRandomInstruction =
            [&](std::string& program, std::uint16_t index)
        { program[index] = GetRandomInstruction(); };

        static const std::array<std::function<void(std::string&, std::uint16_t)>, 3> mutationFunctions = {
            AddRandomInstruction, RemoveInstruction, ReplaceWithRandomInstruction};

        for(size_t i = 0; i < program.length(); ++i)
        {
            if(_mutationRate >= get_random<float>(0.0, 1.0))
            {
                const auto mutation_type =
                    get_random<std::size_t>(0, mutationFunctions.size() - 1);
                mutationFunctions.at(mutation_type)(program, i);
            }
        }
        return program;
    }

    double GeneticCodeGenerator::GetTotalPopulationScore() const
    {
        return std::accumulate(
            _population.cbegin(),
            _population.cend(),
            0,
            [](const double accumulator, const Genotype& g)
            { return accumulator + g.score; });
    }

    Genotype& GeneticCodeGenerator::SelectParent(const std::string otherParent)
    {
        const auto selectionThreshhold = get_random<double>(0.0, 1.0);
        const double totalScore = GetTotalPopulationScore();
        double previousChance = 0;
        for(auto& g: _population)
        {
            double currentChance = g.score / totalScore + previousChance;

            if(currentChance > selectionThreshhold && g.program != otherParent)
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
        const auto crosspoint = get_random<std::size_t>(1, maxStr.length() - 1);

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
                   _population.cbegin(),
                   _population.cend(),
                   [&](const Genotype& g)
                   { return g.program == program; }) != _population.cend();
    }
}