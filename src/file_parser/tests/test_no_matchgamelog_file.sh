#!/bin/bash

# Calling with a file that is not a MatchGameLog_ should return 1
./file_parser ./file_parser;    RESULT_01=$?
./file_parser ../../../test_data/Match_GameLog_TEST01.log; RESULT_02=$?

test $RESULT_01 -eq 1 && test $RESULT_02 -eq 1
