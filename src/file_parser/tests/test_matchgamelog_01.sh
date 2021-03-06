#!/bin/bash

diff <(./file_parser ../../../test_data/Match_GameLog_TEST01.dat) ../../../test_data/Match_GameLog_TEST01.log
RESULT=$?

test $RESULT -eq 0
