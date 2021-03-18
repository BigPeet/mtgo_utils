#ifndef MTGO_UTILS_LIB_MTGO_UTILS_INCLUDE_GAMELOG_GAMELOG_H
#define MTGO_UTILS_LIB_MTGO_UTILS_INCLUDE_GAMELOG_GAMELOG_H

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


/**
 * \brief Return true, if the given file path matches a match game log.
 * \param file_path given file path
 * \return true, if the file path matches.
 *
 * Checks if the file path has the correct extension (.dat) and
 * if the file starts with Match_GameLog_.
 * Does not check any file internals.
 *
 */
bool IsMatchGameLog(std::filesystem::path const& file_path);

} // namespace gl

#endif /* ifndef MTGO_UTILS_LIB_MTGO_UTILS_INCLUDE_GAMELOG_GAMELOG_H */
