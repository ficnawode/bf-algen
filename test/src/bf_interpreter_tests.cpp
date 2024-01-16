
#include "gtest/gtest.h"
#include "bf_interpreter/Interpreter.hpp"

namespace
{
    using namespace bf_interpreter;

    // NOLINTBEGIN
    TEST(interpreter_tests, hello)
    {
        std::string src =
            "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            "++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(Interpreter::Interpret(src), "hello");
    }

    TEST(interpreter_tests, open_brace_error)
    {
        std::string src =
            "[++++[++++][[[]]++++++++++++++++++++++++++++++++++++++++++++++++++"
            "+++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(Interpreter::Interpret(src), std::nullopt);
    }

    TEST(interpreter_tests, closed_brace_error)
    {
        std::string src =
            "[++++[++++][[[]]]]+++++++++++++++]+++++++++++++++++++++++++++++++"
            "+"
            "+"
            "++"
            "+++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(Interpreter::Interpret(src), std::nullopt);
    }

    TEST(interpreter_tests, closed_brace_nonerror)
    {
        std::string src =
            "]]]]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            "++"
            "+"
            "+++++++++++++++++++++++++++++++++++++++.---.+++++++..+++.";
        ASSERT_EQ(Interpreter::Interpret(src), "hello");
    }

    TEST(interpreter_tests, a_loop_test)
    {
        std::string src = "++++++++ [>++++++++<-]>+.";
        ASSERT_EQ(Interpreter::Interpret(src), "A");
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
        ASSERT_EQ(Interpreter::Interpret(src), "Hello, World!");
    }
    // NOLINTEND
}