#!/usr/bin/env bash

if [[ -d ./build ]]; then
  rm -rf ./build
fi

if [[ -d ./install ]]; then
  rm -rf ./install
fi

if [[ -d ./.clangd ]]; then
  rm -rf ./.clangd
fi

if [[ -d ./.cache ]]; then
  rm -rf ./.cache
fi

if [[ -L compile_commands.json ]]; then
  rm compile_commands.json
fi
