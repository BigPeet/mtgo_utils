#!/bin/bash

diff <(./file_parser ../../../test_data/Match_GameLog_TEST03.dat) ../../../test_data/Match_GameLog_TEST03.log
RESULT=$?

test $RESULT -eq 0
