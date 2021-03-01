#include <filesystem>
#include <iostream>

#include "gamelog/gamelog.h"

static void show_usage(std::string const& exec_name)
{
    std::cerr
        << "Usage: " << exec_name << " [OPTIONS] TARGET\n"
        << "\tTARGET\t\tEither a single match game log file to parse or a directory.\n"
        << "\t\t\tThe file should be a Match_GameLog_$ID.dat file from the MTGO directory.\n"
        << "\t\t\tIf a directory is given, all Match_GameLog_$ID.dat files inside it are going to "
        << "be parsed.\n"
        << "Options:\n"
        << "\t-h, --help\t\tShow this help message.\n"
        << "\t-o, --output\t\tIf set, the output will be written to file(s) instead of stdout.\n"
        << "\t\t\t\tThe resulting file name(s) will be Match_GameLog_$ID.log with the $ID of the "
        << "parsed match.\n"
        << "\t-t, --target TARGET_DIR\tIf set, output file(s) will be written into this "
        << "directory.\n"
        << "\t\t\t\tIf TARGET_DIR does not exist, it will be created.\n";
}


static void ParseFile(std::filesystem::path const& to_parse, std::filesystem::path const& output)
{
    auto const lines = gl::ParseGameLogFile(to_parse);
    if (output.empty())
    {
        std::cout << "Parsing " << to_parse.filename() << ":\n";
        for (auto const& line : lines)
        {
            std::cout << line << "\n";
        }
    }
    else
    {
        // TODO: write output to file
    }
}


static bool IsMatchGameLog(std::filesystem::path const& file_path)
{
    return (file_path.extension() == ".dat") &&
           (file_path.filename().generic_string().find("Match_GameLog_") == 0);
}


int main(int argc, char* const* argv)
{
    if (argc < 2)
    {
        show_usage(argv[0]); // NOLINT
        return 1;
    }

    std::filesystem::path file_name;
    std::string output;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i)
    {
        std::string param = argv[i]; // NOLINT
        if ((param == "-h") || (param == "--help"))
        {
            show_usage(argv[0]); // NOLINT
            return 0;
        }
        if ((param == "-o") || (param == "--output-prefix"))
        {
            if (i + 1 < argc)
            {
                output = argv[i++]; // NOLINT
            }
            else
            {
                std::cerr << "Error: --output option requires one argument.\n";
                show_usage(argv[0]); // NOLINT
                return 1;
            }
        }
        else
        {
            file_name = argv[i]; // NOLINT
        }
    }

    if (std::filesystem::is_directory(file_name))
    {
        // Iterate through directory and parse all match game logs.
        for (auto const& file : std::filesystem::recursive_directory_iterator{file_name})
        {
            if ((std::filesystem::is_regular_file(file)) && (IsMatchGameLog(file)))
            {
                ParseFile(file, output);
            }
        }
    }
    else if (std::filesystem::is_regular_file(file_name))
    {
        // Parse given file (if it is a match game log).
        if (!IsMatchGameLog(file_name))
        {
            std::cerr << "Error: " << file_name << " is (likely) no MTGO match game log.\n";
            return 1;
        }
        ParseFile(file_name, std::filesystem::path{output});
    }
    else
    {
        std::cerr << "Error: The given file " << file_name
                  << " is neither a directory nor a regular file.\n";
        return 1;
    }
    return 0;
}
