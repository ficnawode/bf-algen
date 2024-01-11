#include "bf_code_generator/GeneticCodeGenerator.hpp"
#include <algorithm>
#include "bf_interpreter/Interpreter.hpp"
#include "util/util.hpp"

#include <fstream>
#include <iostream>

namespace bf_code_generator
{
    using namespace util;

    GeneticCodeGenerator::GeneticCodeGenerator(const std::string goalOutput) :
        _goalOutput{goalOutput}
    {
        for(int i = 0; i < _populationSize; i++)
        {
            _population.push_back(Genotype(CreateRandomProgram(), CalculateFitness));
        }
    }

    void GeneticCodeGenerator::Run()
    {
        std::string best_program;

        bool keep_going = false; // Just used to have the program keep searching after a match is found.

        unsigned long generations = 0;

        while(1)
        {
            SortPopulation();
            int worst_program_index = _population.size() - 1;
            best_program = _population[0].GetProgram();

            // crossover
            for(int i = 0; i < _numOfCrossovers; i++)
            {
                auto& parent1 = SelectParent();
                auto& parent2 = SelectParent(parent1.GetProgram());

                auto children = Mate(parent1.GetProgram(), parent2.GetProgram());
                parent1.SetProgram(children[0]);
                parent2.SetProgram(children[1]);
            }

#pragma omp parralel for
            for(auto& g: _population)
            {
                g.SetProgram(Mutate(g.GetProgram()));
            }

            // elitism
            if(!ProgramExists(best_program))
                _population[worst_program_index].SetProgram(best_program);

            if(!(generations % _displayRate))
            {
                std::cout << "\n\nBest program evolved so far: " << std::endl;
                std::cout << best_program << std::endl;

                auto output = bf_interpreter::Interpreter::Interpret(best_program);
                std::cout << "\nOutput: " << output.value_or("Error") << std::endl;

                if(output == _goalOutput && !keep_going)
                {
                    std::cout << "\n\a\a\aProgram evolved!" << std::endl;
                    std::cout << "Save source code as a text file? (y/n) ";

                    char answer;
                    std::cin >> answer;

                    if(answer == 'y')
                    {
                        std::ofstream srcfile("bfsrc.txt");
                        srcfile << _goalOutput << ":\n\n" << best_program;
                        std::cout << "Source code saved as 'bfsrc.txt'\n"
                                  << std::endl;
                    }

                    std::cout << "Keep evolving for more efficiency? (y/n) ";
                    std::cin >> answer;

                    if(answer != 'y')
                        return;

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

    std::string GeneticCodeGenerator::Mutate(std::string program)
    {
        for(size_t i = 0; i < program.length(); ++i)
        {
            if(_mutationRate >= get_random_real<float>(0.0, 1.0))
            {
                unsigned mutation_type =
                    get_random_int<unsigned>(0, _mutationFunctions.size() - 1);
                program = _mutationFunctions[mutation_type](program, i);
            }
        }
        return program;
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
        std::sort(_population.begin(), _population.end(), std::greater<Genotype&>());
    }

    Genotype& GeneticCodeGenerator::SelectParent(const std::string other_parent)
    {
        double program_chances[_populationSize]; // Holds each program's chance of being selected (a # between 0 and 1).
        double score_total = GetTotalPopulationScore();
        double rand_val = get_random_real<float>(0.0, 1.0);

        for(int i = 0; i < _populationSize; ++i)
        {
            double prev_chance = ((i - 1) < 0) ? 0 : program_chances[i - 1];

            program_chances[i] = (_population[i].GetScore() / score_total) + (prev_chance);

            if(program_chances[i] > rand_val - 0.001 && _population[i].GetProgram() != other_parent)
                return _population[i];
        }

        return _population[0];
    }

    std::array<std::string, 2>
        GeneticCodeGenerator::Mate(const std::string& parent1, const std::string& parent2)
    {
        // We need to find which program is longest.
        std::string min_str = (parent1.length() < parent2.length()) ? parent1 : parent2;
        std::string max_str = (parent1.length() >= parent2.length()) ? parent1 : parent2;

        // Determine a crossover point at random.
        unsigned crosspoint = get_random_int<unsigned>(1, max_str.length() - 1);

        // Find the substring of the program after the crossover point
        std::string max_str_contrib = max_str.substr(crosspoint);

        // Then erase after that point
        max_str.erase(crosspoint);

        /* If the cross-over point is less than the length of the smaller program, then we need to combine part of it with the larger program.
           If not, then we do nothing and just take part of the larger program and add it to it. */
        if(crosspoint <= min_str.length())
        {
            max_str += min_str.substr(crosspoint);
            min_str.erase(crosspoint);
        }

        // Add the 2nd part of the larger program to the smaller program.
        min_str += max_str_contrib;

        // Call the mutate function on the children which has a small chance of actually causing a mutation.
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