#!/bin/bash

diff <(./file_parser ../../../test_data/Match_GameLog_TEST04.dat) ../../../test_data/Match_GameLog_TEST04.log
RESULT=$?

test $RESULT -eq 0
