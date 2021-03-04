#!/usr/bin/env bash

if [[ ! -d ./build ]]; then
  mkdir build
fi

# Note:
# Set CMAKE_PREFIX_PATH to find packages/programs, like IWYU.
# Set CMAKE_INSTALL_PREFIX to change where targets are installed.
cd build
cmake .. $@
cmake --build .
cd ..

if [[ ! -f compile_commands.json ]]; then
  ln -s build/compile_commands.json .
fi
