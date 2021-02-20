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

    gl::Parse(argv[1]);

    return 0;
}
