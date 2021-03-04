#ifndef GAMELOG_H_INCLUDE
#define GAMELOG_H_INCLUDE

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace gl {

/**
 * \brief Take the given input file, parse its content and return the parsed lines.
 * \param file_path The std::filesystem::path of the Match_GameLog_*.dat file to parse.
 * \return vector of parsed lines
 */
std::vector<std::string> ParseGameLogFile(std::filesystem::path const& file_path);

/**
 * \brief Take the given input file, parse its content and return the parsed lines.
 * \param file_stream The std::ifstream of the Match_GameLog_*.dat file to parse.
 * \return vector of parsed lines
 */
std::vector<std::string> ParseGameLogFile(std::ifstream&& file_stream);

/**
 * \brief Take the given input, parse it and return the parsed lines.
 * \param file_content The content of the Match_GameLog_*.dat file to parse.
 * \return vector of parsed lines
 */
std::vector<std::string> ParseGameLogFile(std::string const& file_content);

} // namespace gl

#endif /* ifndef GAMELOG_H_INCLUDE */
