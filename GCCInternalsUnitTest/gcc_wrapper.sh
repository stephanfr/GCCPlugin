#!/bin/bash

TEST_DIRECTORY=$1
TEST_NAME=$2

if [  ! -d "temp" ]; then
    /bin/mkdir temp
fi

if [  ! -d "results" ]; then
    /bin/mkdir results
 fi

if [  ! -d "results/$TEST_DIRECTORY" ]; then
    /bin/mkdir results/$TEST_DIRECTORY
fi


cd temp

if [ $# -eq 2 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 -fdump-tree-gimple-raw -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -o $TEST_DIRECTORY_$TEST_NAME.junk \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

elif [ $# -eq 3 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 -fdump-tree-gimple-raw -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$3 \
                                                             -o $TEST_DIRECTORY_$TEST_NAME.junk \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

elif [ $# -eq 4 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 -fdump-tree-gimple-raw -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$3 \
                                                            -fplugin-arg-libGCCInternalsUTFixture-$4 \
                                                              -o $TEST_DIRECTORY_$TEST_NAME.junk \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

elif [ $# -eq 5 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 -fdump-tree-gimple-raw -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$3 \
                                                            -fplugin-arg-libGCCInternalsUTFixture-$4 \
                                                            -fplugin-arg-libGCCInternalsUTFixture-$5 \
                                                              -o $TEST_DIRECTORY_$TEST_NAME.junk \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

fi

cd ..

# /bin/rm -f ./temp/*.*

cmp -s ./results/$TEST_DIRECTORY/$TEST_NAME.xml ./expectedResults/$TEST_DIRECTORY/$TEST_NAME.xml

exit $?

