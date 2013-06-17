#!/bin/bash

TEST_DIRECTORY=$1
TEST_NAME=$2

cd temp

/usr/gcc-4.8.0/bin/g++-4.8.0 -S -O0 -MMD -MP\
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-outputFilename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -o $TEST_DIRECTORY_$TEST_NAME.junk \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

cd ..

/bin/rm -f ./temp/*.junk

cmp -s ./results/$TEST_DIRECTORY/$TEST_NAME.xml ./expectedResults/$TEST_DIRECTORY/$TEST_NAME.xml

exit $?

