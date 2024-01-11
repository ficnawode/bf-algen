#include <iostream>
#include "bf_code_generator/GeneticCodeGenerator.hpp"
#include "bf_interpreter/Interpreter.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv, argv + argc);
    if(args.size() != 2)
    {
        std::cout << "incorrect number of arguments! use format ./exec <goal "
                     "output>"
                  << std::endl;
        return 1;
    }
    auto gen = bf_code_generator::GeneticCodeGenerator(args[1]);
    gen.Run();
    return 0;
}