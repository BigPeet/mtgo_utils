#ifndef LIB_MTGO_UTILS_INCLUDE_MTGO_UTILS_GAMELOG_H
#define LIB_MTGO_UTILS_INCLUDE_MTGO_UTILS_GAMELOG_H

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace mtgo_utils {

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

/**
 * \brief Return file path of the MTGO installation, if it exists.
 * \param root_dir the root directory (e.g. C:\)
 * \return optional which contains the installation path on success.
 *
 * Searches for the MTGO installation folder under the given root directory.
 * It is expected under Windows that the given root_dir is a drive, e.g. C:\, and contains the
 * 'Users' directory. Under wine this might be 'users'.
 * If the given path is not a existing directory or the installation can not be found, then no value
 * will be returned.
 *
 */
std::optional<std::filesystem::path> FindMTGORoot(std::filesystem::path const& root_dir);

} // namespace mtgo_utils

#endif /* ifndef LIB_MTGO_UTILS_INCLUDE_MTGO_UTILS_GAMELOG_H */
