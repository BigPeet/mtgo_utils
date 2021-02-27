#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
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


std::vector<std::string> ExtractPlayerNames(std::vector<std::string> const& lines)
{
    // Assuming game log starts with
    // Player A rolled N.
    // Player B rolled J.
    // Player C rolled K.
    // ...
    std::unordered_set<std::string> names;
    for (auto const& line : lines)
    {
        auto const pos = line.find(" rolled a ");
        if (pos != std::string::npos)
        {
            names.insert(line.substr(0, pos)); // emplace or insert?
        }
    }
    return std::vector<std::string>{names.cbegin(), names.cend()};
}

void CleanUpTurnLines(std::vector<std::string>& lines, std::vector<std::string> const& players)
{
    for (auto& line : lines)
    {
        if (line.find("Turn ") == 0)
        {
            // starts with "Turn "
            for (auto const& player : players)
            {
                auto const pos = line.find(player);
                if (pos != std::string::npos)
                {
                    // It's that players turn, cut off line after the name
                    line.erase(pos + player.length());
                    break;
                }
            }
        }
    }
}

std::vector<std::string> Split(std::string const& content)
{
    static constexpr char const* sentinel = "@P";
    static constexpr size_t sentinel_length{2};

    std::vector<std::string> tokens;
    size_t pos{content.find(sentinel)};

    while (pos != std::string::npos)
    {
        std::string token;
        size_t const next_pos{content.find(sentinel, pos + sentinel_length)};
        if ((next_pos != std::string::npos) && ((pos + sentinel_length) < next_pos))
        {
            token = content.substr(pos + sentinel_length, (next_pos - pos - sentinel_length + 1));
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
    CleanUpTurnLines(tokens, ExtractPlayerNames(tokens));
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
