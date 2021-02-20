#!/usr/bin/env bash

if [[ -d ./build ]]; then
  rm -rf ./build
fi

if [[ -d ./install ]]; then
  rm -rf ./install
fi

if [[ -L compile_commands.json ]]; then
  rm compile_commands.json
fi
