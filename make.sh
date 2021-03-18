#!/usr/bin/env bash

if [[ ! -d ./build ]]; then
  mkdir build
fi

if [[ $1 == "default" ]]; then
  DEFAULT_CMAKE_ARGS="-DENABLE_TESTING=ON -DENABLE_IWYU=ON -DCMAKE_INSTALL_PREFIX=../install"
  shift
else
  DEFAULT_CMAKE_ARGS=""
fi

# Note:
# Set CMAKE_PREFIX_PATH to find packages/programs, like IWYU.
# Set CMAKE_INSTALL_PREFIX to change where targets are installed.
cd build
cmake .. $DEFAULT_CMAKE_ARGS $@
cmake --build .
cmake --install .
cd ..

if [[ ! -f compile_commands.json ]]; then
  ln -s build/compile_commands.json .
fi
