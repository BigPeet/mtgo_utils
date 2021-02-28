# mtgo_utils
Tooling suite for Magic: The Gathering Online (MTGO)

[![CMake](https://github.com/BigPeet/mtgo_utils/actions/workflows/cmake.yml/badge.svg)](https://github.com/BigPeet/mtgo_utils/actions/workflows/cmake.yml)

# Goals / Milestones

## Game Log (WIP)

* (WIP) Parsing game log files into human readable text.
    * [x] Provide an API parsing a file (path, ifstream, raw text) and returning the parsed lines.
    * [ ] Provide an (CLI) executable which takes an input file, parses it and writes the output either to stdout or into a provided output file.
    * [ ] Provide documentation on how to install/use.
* (Future) Scanning game folder to easily browse log files and detect new ones.
* (Future) Live parsing of game log files.
* (Future) Create actual match data from the parsed game logs.
    * who won
    * which cards were played
    * ...
