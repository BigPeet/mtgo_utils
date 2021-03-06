#!/bin/bash

# Calling with a nonexisting file should return 1
./file_parser MatchGameLog_someRand0m_File.dat; RESULT=$?

test $RESULT -eq 1
