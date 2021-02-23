#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "gamelog/gamelog.h"

namespace gl {

std::string Read(std::string_view file_name)
{
    std::ifstream in{file_name.data()};
    return {std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>()};
}

void CutOff(std::string& sentence)
{
    int bracket_level{0};
    size_t end{sentence.length()};

    for (size_t i{0}; i < sentence.length(); i++)
    {
        char cur{sentence[i]};
        if (cur == '(')
        {
            bracket_level++;
        }
        else if (cur == ')')
        {
            bracket_level--;
        }
        else if (cur == '\n')
        {
            end = i;
            break;
        }
        else if ((cur == '.') && (bracket_level == 0))
        {
            end = i + 1;
            break;
        }
    }
    sentence.erase(end);
}

std::vector<std::string> Split(std::string const& content)
{
    std::vector<std::string> tokens;
    char const* sentinel = "@P";
    size_t const sentinel_length{2};
    size_t const len{content.length()};
    size_t pos{0};
    while ((pos < len) && (pos != std::string::npos))
    {
        pos = content.find(sentinel, pos);
        if (pos != std::string::npos)
        {
            std::string token;
            size_t const next_pos{content.find(sentinel, pos + sentinel_length)};
            if ((next_pos != std::string::npos) && ((pos + sentinel_length) < next_pos))
            {
                token =
                    content.substr(pos + sentinel_length, (next_pos - pos - sentinel_length + 1));
            }
            else if (next_pos == std::string::npos)
            {
                token = content.substr(pos + sentinel_length);
            }

            if (token.length() > 0)
            {
                CutOff(token);
                tokens.push_back(token);
            }

            pos = next_pos;
        }
    }
    return tokens;
}

void Parse(std::string_view file_name)
{
    auto const text   = Read(file_name);
    auto const tokens = Split(text);
    for (auto const& token : tokens)
    {
        std::cout << "Token: " << token << "\n";
    }
}

} // namespace gl
