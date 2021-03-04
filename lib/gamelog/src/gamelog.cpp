#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gamelog/gamelog.h"

namespace gl {

namespace internal {


std::string Read(std::ifstream&& file_stream)
{
    return {std::istreambuf_iterator<char>{file_stream}, std::istreambuf_iterator<char>()};
}

std::string Read(std::filesystem::path const& file_path)
{
    // std::ifstream in{file_path.data()};
    // return Read(in);
    return Read(std::ifstream(file_path));
}


void CutOff(std::string& sentence)
{
    // It seems every actual line in the game log is a sentence ending with a period "."
    // Everything after the period can be discarded.
    // But there might be "sub-sentences" in (), e.g. reminder text.
    // These do not indicate a cut-off.
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
            names.insert(line.substr(0, pos));
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
    // This sentinel seems to be a good indicator for the start of a line.
    static constexpr char const* sentinel = "@P";
    static constexpr size_t sentinel_length{2};

    std::vector<std::string> lines;
    size_t pos{content.find(sentinel)};

    while (pos != std::string::npos)
    {
        // Initialize the next line from the current sentinel to the next.
        // If there is none left, go until the end of the text.
        std::string line;
        size_t const next_pos{content.find(sentinel, pos + sentinel_length)};
        if ((next_pos != std::string::npos) && ((pos + sentinel_length) < next_pos))
        {
            line = content.substr(pos + sentinel_length, (next_pos - pos - sentinel_length + 1));
        }
        else if (next_pos == std::string::npos)
        {
            line = content.substr(pos + sentinel_length);
        }

        // Cut off the line based on punctuation to remove some of the encoding noise.
        // Then add it to the return vector.
        if (line.length() > 0)
        {
            CutOff(line);
            lines.push_back(std::move(line));
        }

        pos = next_pos;
    }

    // Apply additional cut-off to "Turn X: " lines
    CleanUpTurnLines(lines, ExtractPlayerNames(lines));

    return lines;
}

} // namespace internal

std::vector<std::string> ParseGameLogFile(std::filesystem::path const& file_path)
{
    auto const text = gl::internal::Read(file_path);
    return ParseGameLogFile(text);
}

std::vector<std::string> ParseGameLogFile(std::ifstream&& file_stream)
{
    auto const text = gl::internal::Read(std::move(file_stream));
    return ParseGameLogFile(text);
}

std::vector<std::string> ParseGameLogFile(std::string const& file_content)
{
    return gl::internal::Split(file_content);
}

} // namespace gl
