#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mtgo_utils/gamelog.h"

namespace mtgo_utils {

namespace internal {

static std::string Read(std::ifstream&& file_stream)
{
    return {std::istreambuf_iterator<char>{file_stream}, std::istreambuf_iterator<char>()};
}


static std::string Read(std::filesystem::path const& file_path)
{
    return Read(std::ifstream(file_path));
}


constexpr static bool IsPrintableASCII(char c)
{
    constexpr char kMinAscii{31};
    constexpr char kMaxAscii{127};
    return (c > kMinAscii) && (c < kMaxAscii);
}


static void CutOff(std::string& sentence)
{
    // It seems every actual line in the game log is a sentence ending with a period "."
    // Everything after the period can be discarded.
    // But there might be "sub-sentences" in (), e.g. reminder text.
    // These do not indicate a cut-off.
    int bracket_level{0};
    auto const len{sentence.length()};
    auto end{std::string::npos};

    for (size_t i{0}; i < len; i++)
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
            // We found the end of the sentence, but there might be another 'sentence' in this line.
            // This is sometimes the case to specify a value for X.
            // E.g., "Player activates an ability of Blast Zone. (X is 2) ( ... )."
            end = i + 1;

            // Check if another sentence might be following.
            if (((i + 2) > len) || (sentence[i + 1] != ' ') || (!IsPrintableASCII(sentence[i + 2])))
            {
                // Definitive cut off point.
                break;
            }
        }
    }

    // Did we find an end? => erase remainder.
    if (end != std::string::npos)
    {
        sentence.erase(end);
    }
}


static std::vector<std::string> ExtractPlayerNames(std::vector<std::string> const& lines)
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


static void CleanUpTurnLines(std::vector<std::string>& lines,
                             std::vector<std::string> const& players)
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


static std::vector<std::string> Split(std::string const& content)
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
    auto const text = mtgo_utils::internal::Read(file_path);
    return ParseGameLogFile(text);
}


std::vector<std::string> ParseGameLogFile(std::ifstream&& file_stream)
{
    auto const text = mtgo_utils::internal::Read(std::move(file_stream));
    return ParseGameLogFile(text);
}


std::vector<std::string> ParseGameLogFile(std::string const& file_content)
{
    return mtgo_utils::internal::Split(file_content);
}


bool IsMatchGameLog(std::filesystem::path const& file_path)
{
    return (file_path.extension() == ".dat") &&
           (file_path.filename().string().find("Match_GameLog_") == 0);
}


std::optional<std::filesystem::path> FindMTGORoot(std::filesystem::path const& root_dir)
{
    std::optional<std::filesystem::path> res{};
    if (std::filesystem::is_directory(root_dir))
    {
        std::filesystem::path users_dir{root_dir / "Users"};
        if (!std::filesystem::is_directory(users_dir))
        {
            users_dir.replace_filename("users");
        }
        if (std::filesystem::is_directory(users_dir))
        {
            // check if path exists in user directory
            std::filesystem::path const install_dir{"AppData/Local/Apps/2.0"};
            for (auto const& user_dir : std::filesystem::directory_iterator(users_dir))
            {
                std::filesystem::path complete_path{user_dir.path() / install_dir};
                if (std::filesystem::is_directory(complete_path))
                {
                    res.emplace(std::move(complete_path));
                    break;
                }
            }
        }
    }
    return res;
}

} // namespace mtgo_utils
