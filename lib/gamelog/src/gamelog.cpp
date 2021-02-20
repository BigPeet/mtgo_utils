#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "gamelog/gamelog.h"

namespace gl {

void Parse(std::string_view file_name)
{
    std::ifstream in{file_name.data()};
    if (in.is_open())
    {
        std::string buffer;
        while (std::getline(in, buffer))
        {
            std::cout << buffer;
        }
    }
}

} // namespace gl
