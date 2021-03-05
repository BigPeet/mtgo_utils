#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

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
        << "directory (default: current working directory).\n"
        << "\t\t\t\tIf TARGET_DIR does not exist, this tool will try to create it.\n";
}


static void ParseFile(std::filesystem::path const& to_parse, std::filesystem::path const& output)
{
    std::cout << "Parsing " << to_parse.filename() << ":\n";
    auto const lines = gl::ParseGameLogFile(to_parse);

    std::ofstream out{output};
    for (auto const& line : lines)
    {
        out << line << "\n";
    }
    std::cout << "Results written to " << output << "\n";
}


static void ParseFile(std::filesystem::path const& to_parse)
{
    std::cout << "Parsing " << to_parse.filename() << ":\n";
    auto const lines = gl::ParseGameLogFile(to_parse);
    for (auto const& line : lines)
    {
        std::cout << line << "\n";
    }
}


static bool CreateDirectories(std::filesystem::path const& dir_path)
{
    std::error_code dir_error;
    std::filesystem::create_directories(dir_path, dir_error);
    if (dir_error.value() != 0)
    {
        std::cerr << "Error: Could not create directories: " << dir_path << ": "
                  << dir_error.message() << "\n";
        return false;
    }
    return true;
}


int main(int argc, char* const* argv)
{
    if (argc < 2)
    {
        show_usage(argv[0]); // NOLINT
        return 1;
    }

    std::filesystem::path file_name{};
    std::filesystem::path target{std::filesystem::current_path()};
    bool output_to_file{false};

    // Parse command line arguments
    for (int i{1}; i < argc; i++)
    {
        std::string param{argv[i]}; // NOLINT
        if ((param == "-h") || (param == "--help"))
        {
            show_usage(argv[0]); // NOLINT
            return 0;
        }
        if ((param == "-o") || (param == "--output"))
        {
            output_to_file = true;
        }
        else if ((param == "-t") || (param == "--target"))
        {
            if ((i + 1) < argc)
            {
                target = std::filesystem::path{argv[++i]}; // NOLINT
            }
            else
            {
                std::cerr << "Error: --target option requires one argument.\n";
                show_usage(argv[0]); // NOLINT
                return 1;
            }
        }
        else
        {
            file_name = std::filesystem::path{argv[i]}; // NOLINT
        }
    }

    target /= "placeholder.dat"; // setup target for replacement ooperations
    if (output_to_file)
    {
        // Create directories for output files.
        if (!CreateDirectories(target.parent_path()))
        {
            return 1;
        }
    }

    if (std::filesystem::is_directory(file_name))
    {
        // Iterate through directory and parse all match game logs.
        for (auto const& file : std::filesystem::recursive_directory_iterator{file_name})
        {
            if ((std::filesystem::is_regular_file(file)) && (gl::IsMatchGameLog(file)))
            {
                if (output_to_file)
                {
                    target.replace_filename(file.path().filename()).replace_extension(".log");
                    ParseFile(file, target);
                }
                else
                {
                    ParseFile(file);
                }
            }
        }
    }
    else if (std::filesystem::is_regular_file(file_name))
    {
        // Parse given file (if it is a match game log).
        if (!gl::IsMatchGameLog(file_name))
        {
            std::cerr << "Error: " << file_name << " is (likely) no MTGO match game log.\n";
            return 1;
        }
        if (output_to_file)
        {
            ParseFile(file_name,
                      target.replace_filename(file_name.filename()).replace_extension(".log"));
        }
        else
        {
            ParseFile(file_name);
        }
    }
    else
    {
        std::cerr << "Error: The given file " << file_name
                  << " is neither a directory nor a regular file.\n";
        return 1;
    }

    return 0;
}
