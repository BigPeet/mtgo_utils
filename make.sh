#!/usr/bin/env bash

if [[ ! -d ./build ]]; then
  mkdir build
fi

cd build
cmake .. $@
cmake --build .
cd ..

if [[ ! -f compile_commands.json ]]; then
  ln -s build/compile_commands.json .
fi
