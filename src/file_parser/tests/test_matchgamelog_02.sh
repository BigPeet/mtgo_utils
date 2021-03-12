#!/bin/bash

diff <(./file_parser ../../../test_data/Match_GameLog_TEST02.dat) ../../../test_data/Match_GameLog_TEST02.log
RESULT=$?

test $RESULT -eq 0
