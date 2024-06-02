#pragma once

#include <string>
#include <vector>

inline std::vector<std::string> split(std::string &s, char delimiter)
{
    std::vector<std::string> tokens;

    int start = 0, end;
    while ((end = s.find(delimiter, start)) != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(s.substr(start));

    return tokens;
}