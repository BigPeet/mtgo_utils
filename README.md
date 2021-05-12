# mtgo\_utils

Tooling suite for Magic: The Gathering Online (MTGO)

[![CMake](https://github.com/BigPeet/mtgo_utils/actions/workflows/cmake.yml/badge.svg)](https://github.com/BigPeet/mtgo_utils/actions/workflows/cmake.yml)

# Features

## Game Log

* Parsing MTGO match game log files into human readable text.
    * The `file_parser` tool parses either a single Match\_GameLog\_\*.dat file or all of those files in a given directory.
    * The human readable content is either written to "stdout" or to a corresponding Match\_GameLog\_\*.log file.
    * For more information, read the instructions provided by running `file_parser --help`.

# Installation

## Using pre-built binaries

Download the correct archive (Linux or Windows) from the releases.
Extract the content into a directory (e.g. mtgo\_utils).

On Windows, the executables (.exe) files should be ready to run.

On Linux, you will need to add the .so files to the search path, so they can be loaded, when an executable is run.
This can be achieved by either copying the .so file into a default directory (e.g. /usr/lib) or by adding the directory containing the .so files to the LD\_LIBRARY\_PATH.

## Building and Installing from Source

The code should be compatible with Linux and Windows (MSVC).
cmake is used to configure the build environment and should be able to find the correct compiler and build tools.

```bash
git clone https://github.com/BigPeet/mtgo_utils
cd mtgo_utils
mkdir build && cd build

# If you want to control, where the binaries will be installed
# you can set CMAKE_INSTALL_PREFIX here.
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install .
```

For the executables to properly work, the location of the shared libraries (.so on Linux and .dll Windows) needs to be added to the search path.
On Windows that should be the PATH-environment.
(It also might be sufficient to put the .dll file into the same directory as the executable.)
On Linux the installation directory can be added to the LD\_LIBRARY\_PATH.

# Development

## Current Work in Progress

### Game Log

* Parsing game log files into human readable text.
    * [x] Provide an API parsing a file (path, ifstream, raw text) and returning the parsed lines.
    * [x] Provide an (CLI) executable which takes an input file, parses it and writes the output either to stdout or into a provided output file.
    * [x] Provide documentation on how to install/use.
* Finding and scanning game folder to easily browse log files and detect new ones.
    * [x] Provide an API to find the MTGO installation folder.
    * [ ] Provide an API to find and filter (time stamp, location) game logs inside an installation folder.
* (Future) Live parsing of game log files.
* (Future) Extract actual match data from the parsed game logs.
    * who won/lost (or is it still running => if yes what game of the match)
    * which cards were played
    * Game ID, Player Names
    * ...
    * Store this data somehow
