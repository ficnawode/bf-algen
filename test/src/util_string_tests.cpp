
#include <gtest/gtest.h>
#include "util/string.hpp"

using namespace util;

TEST(string_tests, longer_test)
{
    static constexpr std::string s1 = "helooo";
    static constexpr std::string s2 = "haii";
    EXPECT_EQ(s1, get_longer_string(s1, s2));
    EXPECT_EQ(s1, get_longer_string(s2, s1));
}

TEST(string_tests, shorter_test)
{
    static constexpr std::string s1 = "helooo";
    static constexpr std::string s2 = "haii";
    EXPECT_EQ(s2, get_shorter_string(s1, s2));
    EXPECT_EQ(s2, get_shorter_string(s2, s1));
}

TEST(string_tests, eq_test)
{
    static constexpr std::string s1 = "helo";
    static constexpr std::string s2 = "haii";
    EXPECT_EQ(s1, get_longer_string(s1, s2));
    EXPECT_EQ(s1, get_longer_string(s2, s1));
    EXPECT_EQ(s2, get_shorter_string(s1, s2));
    EXPECT_EQ(s2, get_shorter_string(s2, s1));
}