#include <filesystem>
#include <iostream>

#include "gamelog/gamelog.h"

int main(int argc, char* const* argv)
{
    if (argc != 2)
    {
        std::cout << "This executabel takes exactly one argument.\n";
        std::cout << "Usage: file_parser FILE_PATH\n";
        return 0;
    }

    std::string file_name{argv[1]}; // NOLINT
    std::filesystem::path to_parse{file_name};
    if (to_parse.extension() != ".dat")
    {
        std::cout << "Invalid extension: " << to_parse.extension() << "\n";
    }
    auto const lines = gl::ParseGameLogFile(to_parse);
    for (auto const& line : lines)
    {
        std::cout << line << "\n";
    }

    return 0;
}
