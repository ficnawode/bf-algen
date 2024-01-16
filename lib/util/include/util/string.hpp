#pragma once
#include <cstdint>
#include <string>

namespace util
{
    inline std::string get_shorter_string(const std::string& s1, const std::string& s2)
    {
        if(s1.length() < s2.length())
            return s1;
        return s2;
    }

    inline std::string get_longer_string(const std::string& s1, const std::string& s2)
    {
        if(s1.length() >= s2.length())
            return s1;
        return s2;
    }

    double string_distance(const std::string& s1, const std::string& s2)
    {
        const std::string minStr = get_shorter_string(s1, s2);
        const std::string maxStr = get_longer_string(s1, s2);

        double score = 0;
        for(std::size_t i = 0; i < maxStr.length(); i++)
        {
            uint16_t minChar;
            if(i < minStr.length())
            {
                minChar = minStr[i];
            }
            else
            {
                minChar = maxStr[i] + 255;
            }
            score += abs(maxStr[i] - minChar);
        }
        return score;
    }

}