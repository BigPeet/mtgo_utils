#ifndef GAMELOG_H_INCLUDE
#define GAMELOG_H_INCLUDE

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace gl {

std::vector<std::string> ParseGameLogFile(std::filesystem::path const& file_path);
std::vector<std::string> ParseGameLogFile(std::ifstream&& file_stream);
std::vector<std::string> ParseGameLogFile(std::string const& file_content);

} // namespace gl

#endif /* ifndef GAMELOG_H_INCLUDE */
