#include <gtest/gtest.h>
#include "util/random.hpp"

TEST(rand_tests, float_test)
{
    static constexpr float min = -33.5;
    static constexpr float max = 77.6743;
    for(int i = 0; i < 500; i++)
    {
        float val = util::get_random<float>(min, max);
        EXPECT_LT(val, max);
        EXPECT_GE(val, min);
    }
}

TEST(rand_tests, double_test)
{
    static constexpr double min = -33879876.5;
    static constexpr double max = 987877.6743;
    for(int i = 0; i < 500; i++)
    {
        double val = util::get_random<double>(min, max);
        EXPECT_LT(val, max);
        EXPECT_GE(val, min);
    }
}

TEST(rand_tests, uint8_test)
{
    static constexpr uint8_t min = 0;
    static constexpr uint8_t max = 255;
    for(int i = 0; i < 500; i++)
    {
        uint8_t val = util::get_random<uint8_t>(min, max);
        EXPECT_LE(val, max);
        EXPECT_GE(val, min);
    }
}

TEST(rand_tests, int64_test)
{
    static constexpr int64_t min = -1000;
    static constexpr int64_t max = 255000;
    for(int i = 0; i < 500; i++)
    {
        int64_t val = util::get_random<int64_t>(min, max);
        EXPECT_LE(val, max);
        EXPECT_GE(val, min);
    }
}