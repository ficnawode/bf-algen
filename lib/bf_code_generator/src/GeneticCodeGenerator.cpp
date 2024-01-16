#include "bf_code_generator/GeneticCodeGenerator.hpp"

#include <algorithm>
#include <chrono>
#include "bf_interpreter/Interpreter.hpp"
#include "util/random.hpp"

#include <iostream>

namespace bf_code_generator
{
    using namespace util;

    GeneticCodeGenerator::GeneticCodeGenerator(const std::string goalOutput) :
        _goalOutput{goalOutput},
        CalculateFitness{[&](const std::string& program)
                         {
                             auto interpreter_output =
                                 bf_interpreter::Interpreter::Interpret(program);
                             if(!interpreter_output.has_value())
                                 return _errorScore;
                             auto output = interpreter_output.value();

                             std::string minStr, maxStr;
                             if(output.length() < _goalOutput.length())
                             {
                                 minStr = output;
                                 maxStr = _goalOutput;
                             }
                             else
                             {
                                 minStr = _goalOutput;
                                 maxStr = output;
                             }

                             double score = 0;
                             for(int i = 0; i < maxStr.length(); i++)
                             {
                                 uint16_t minChar;
                                 if(i < minStr.length())
                                 {
                                     minChar = minStr[i];
                                 }
                                 else
                                 {
                                     minChar = maxStr[i] + CHAR_SIZE;
                                 }
                                 score += abs(maxStr[i] - minChar);
                             }
                             score += (program.length() * _lengthPenalty);

                             double maxScore = _goalOutput.length() * CHAR_SIZE;
                             return maxScore - score;
                         }}
    {
        for(int i = 0; i < _populationSize; i++)
        {
            _population.push_back(Genotype(CreateRandomProgram(), CalculateFitness));
        }
    }

    void GeneticCodeGenerator::Run()
    {
        bool keep_going = false;
        auto start_time = std::chrono::high_resolution_clock::now();

        for(unsigned long generations = 0;; generations++)
        {
            SortPopulation();
            int worst_program_index = _population.size() - 1;
            std::string best_program = _population[0].GetProgram();

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
            if(!ProgramExists(best_program))
            {
                _population[worst_program_index].SetProgram(best_program);
            }

            if(!(generations % _displayRate))
            {
                std::cout << "\n\nCurrent best program: " << std::endl;
                std::cout << best_program << std::endl;

                auto output = bf_interpreter::Interpreter::Interpret(best_program);
                std::cout << "\nOutput: " << output.value_or("Error") << std::endl;
                std::cout << "Value of loss function: " << CalculateFitness(best_program)
                          << std::endl;

                if(output == _goalOutput && !keep_going)
                {
                    std::cout << "\n\a\a\aProgram evolved." << std::endl;
                    auto end_time = std::chrono::high_resolution_clock::now();

                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time);

                    std::cout << "Time taken: " << duration.count()
                              << " milliseconds" << std::endl;
                    std::cout << "Number of generations: " << generations << std::endl;

                    char answer;
                    std::cout << "Keep evolving for more efficiency? (y/n) ";
                    std::cin >> answer;

                    if(answer != 'y')
                        return;
                    _mutationRate = 0.05;
                    _lengthPenalty = 0.01;

                    keep_going = true;
                }
            }

            ++generations;
        }
    }

    char GeneticCodeGenerator::GetRandomInstruction()
    {
        auto min = 0;
        auto max = Instructions.size();
        auto index = get_random<size_t>(min, max);
        return Instructions[index];
    }

    std::string GeneticCodeGenerator::CreateRandomProgram() const
    {
        std::string program;
        uint16_t program_size = get_random<uint16_t>(_minProgramSize, _maxProgramSize);

        for(unsigned i = 0; i < program_size; ++i)
        {
            program += GetRandomInstruction();
        }
        return program;
    }

    std::string GeneticCodeGenerator::Mutate(std::string program)
    {
        static const std::function<std::string(const std::string, unsigned)> AddRandomInstruction =
            [&](std::string program, unsigned index)
        {
            if((program.length()) < _maxProgramSize)
                program.insert(index, 1, GetRandomInstruction());
            return program;
        };

        static const std::function<std::string(const std::string, unsigned)> RemoveInstruction =
            [&](std::string program, unsigned index)
        {
            if(program.length() > _minProgramSize)
                program.erase(index, 1);
            return program;
        };

        static const std::function<std::string(const std::string, unsigned)> ReplaceWithRandomInstruction =
            [&](std::string program, unsigned index)
        {
            program[index] = GetRandomInstruction();
            return program;
        };

        static const std::array<std::function<std::string(const std::string, unsigned)>, 3> mutationFunctions = {
            AddRandomInstruction, RemoveInstruction, ReplaceWithRandomInstruction};
        for(size_t i = 0; i < program.length(); ++i)
        {
            if(_mutationRate >= get_random<float>(0.0, 1.0))
            {
                unsigned mutation_type =
                    get_random<unsigned>(0, mutationFunctions.size() - 1);
                program = mutationFunctions[mutation_type](program, i);
            }
        }
        return program;
    }

    double GeneticCodeGenerator::GetTotalPopulationScore() const
    {
        auto add_lambda = [](double accumulator, const Genotype& g)
        { return accumulator + g.GetScore(); };
        return std::accumulate(_population.begin(), _population.end(), 0, add_lambda);
    }

    void GeneticCodeGenerator::SortPopulation()
    {
        std::sort(_population.begin(), _population.end(), std::greater<Genotype&>());
    }

    Genotype& GeneticCodeGenerator::SelectParent(const std::string other_parent)
    {
        std::vector<double> program_chances(_populationSize);
        double score_total = GetTotalPopulationScore();
        double selectionThreshhold = get_random<float>(0.0, 1.0);

        double previousChance = 0;
        for(int i = 0; i < _populationSize; ++i)
        {
            double currentChance = _population[i].GetScore() / score_total + previousChance;

            if(currentChance > selectionThreshhold && _population[i].GetProgram() != other_parent)
                return _population[i];
            previousChance = currentChance;
        }

        return _population[0];
    }

    std::array<std::string, 2>
        GeneticCodeGenerator::Mate(const std::string& parent1, const std::string& parent2)
    {
        std::string min_str = (parent1.length() < parent2.length()) ? parent1 : parent2;
        std::string max_str = (parent1.length() >= parent2.length()) ? parent1 : parent2;

        unsigned crosspoint = get_random<unsigned>(1, max_str.length() - 1);

        std::string max_str_contrib = max_str.substr(crosspoint);

        max_str.erase(crosspoint);

        if(crosspoint <= min_str.length())
        {
            max_str += min_str.substr(crosspoint);
            min_str.erase(crosspoint);
        }

        min_str += max_str_contrib;

        return {min_str, max_str};
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
}