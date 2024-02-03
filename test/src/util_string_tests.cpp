
#include <gtest/gtest.h>
#include "util/string.hpp"

using namespace util;
namespace
{
    // NOLINTBEGIN
    TEST(string_tests, longer_test)
    {
        std::string s1 = "helooo";
        std::string s2 = "haii";
        EXPECT_EQ(s1, get_longer_string(s1, s2));
        EXPECT_EQ(s1, get_longer_string(s2, s1));
    }

    TEST(string_tests, shorter_test)
    {
        std::string s1 = "helooo";
        std::string s2 = "haii";
        EXPECT_EQ(s2, get_shorter_string(s1, s2));
        EXPECT_EQ(s2, get_shorter_string(s2, s1));
    }

    TEST(string_tests, eq_test)
    {
        std::string s1 = "helo";
        std::string s2 = "haii";
        EXPECT_EQ(s1, get_longer_string(s1, s2));
        EXPECT_EQ(s2, get_longer_string(s2, s1));
        EXPECT_EQ(s2, get_shorter_string(s1, s2));
        EXPECT_EQ(s1, get_shorter_string(s2, s1));
    }

    TEST(string_tests, distance_test)
    {
        std::string s1 = "helo";
        std::string s2 = "hi";
        EXPECT_EQ(0, string_distance(s1, s1));
        EXPECT_EQ(0, string_distance(s2, s2));
        EXPECT_EQ(514, string_distance(s1, s2));
    }
    // NOLINTEND
}