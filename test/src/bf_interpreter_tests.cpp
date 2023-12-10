
#include "gtest/gtest.h"
#include "bf/Interpreter.hpp"

namespace
{

    TEST(interpreter_tests, hello)
    {
        std::string src =
            "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            "++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        EXPECT_EQ(bf::Interpreter::Interpret(src), "hello");
    }
    TEST(interpreter_tests, hello_world)
    {
        std::string src =
            ">++++++++[<+++++++++>-]<."
            ">++++[<+++++++>-]<+."
            "+++++++.."
            "+++."
            ">>++++++[<+++++++>-]<++."
            "------------."
            ">++++++[<+++++++++>-]<+."
            "<."
            "+++."
            "------."
            "--------."
            ">>>++++[<++++++++>-]<+.";
        EXPECT_EQ(bf::Interpreter::Interpret(src), "Hello, World!");
    }
}