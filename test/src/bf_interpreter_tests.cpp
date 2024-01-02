
#include "gtest/gtest.h"
#include "bf/Interpreter.hpp"

namespace
{

    TEST(interpreter_tests, hello)
    {
        std::string src =
            "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            "++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(bf::Interpreter::Interpret(src), "hello");
    }

    TEST(interpreter_tests, open_brace_error)
    {
        std::string src =
            "[++++[++++][[[]]++++++++++++++++++++++++++++++++++++++++++++++++++"
            "+++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(bf::Interpreter::Interpret(src), std::nullopt);
    }

    TEST(interpreter_tests, closed_brace_error)
    {
        std::string src =
            "[++++[++++][[[]]]]+++++++++++++++]+++++++++++++++++++++++++++++++"
            "+"
            "+"
            "++"
            "+++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(bf::Interpreter::Interpret(src), std::nullopt);
    }

    TEST(interpreter_tests, closed_brace_nonerror)
    {
        std::string src =
            "]]]]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            "++"
            "+"
            "+++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(bf::Interpreter::Interpret(src), "hello");
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
        ASSERT_EQ(bf::Interpreter::Interpret(src), "Hello, World!");
    }
}