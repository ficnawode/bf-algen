#include <iostream>
#include "dummy/dummy.hpp"

int main()
{
    std::cout << "HELLO, WORLD! 5+3 = " << dummy::add(5, 3) << std::endl;
    return 0;
}