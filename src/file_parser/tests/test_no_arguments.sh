#!/bin/bash

# Calling without parameters should return 1
./file_parser; RESULT=$?

test $RESULT -eq 1
