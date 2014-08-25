#!/bin/bash


GCC_OPTIONS=$1
TEST_DIRECTORY=$2
TEST_NAME=$3


if [  ! -d "temp" ]; then
    /bin/mkdir temp
fi

if [  ! -d "results" ]; then
    /bin/mkdir results
 fi

if [  ! -d "results/$TEST_DIRECTORY" ]; then
    /bin/mkdir results/$TEST_DIRECTORY
fi


CREATE_EXE=true

if [[ $GCC_OPTIONS == "-c" || $GCC_OPTIONS == *" -c "* ]]
then
CREATE_EXE=false
fi


if $CREATE_EXE
then
OUTPUT_OPTION="-o ../results/$TEST_DIRECTORY/$TEST_NAME.exe"
else
OUTPUT_OPTION=""
fi



cd temp



if [ $# -eq 3 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 $GCC_OPTIONS -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             $OUTPUT_OPTION \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

elif [ $# -eq 4 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 $GCC_OPTIONS -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$4 \
                                                             $OUTPUT_OPTION \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

elif [ $# -eq 5 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 $GCC_OPTIONS -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$4 \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$5 \
                                                              $OUTPUT_OPTION \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

elif [ $# -eq 6 ]
then

    /usr/gcc-4.8.0/bin/g++-4.8.0 $GCC_OPTIONS -std=c++11 \
							     -fplugin=../../GCCInternalsUTFixture/Debug/libGCCInternalsUTFixture.so \
                                                             -fplugin-arg-libGCCInternalsUTFixture-output-filename=../results/$TEST_DIRECTORY/$TEST_NAME.xml \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$4 \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$5 \
                                                             -fplugin-arg-libGCCInternalsUTFixture-$6 \
                                                              $OUTPUT_OPTION \
                                                             ../testCaseSourceCode/$TEST_DIRECTORY/$TEST_NAME.cpp

fi

cd ..

/bin/rm -f ./temp/*.*


cmp -s ./results/$TEST_DIRECTORY/$TEST_NAME.xml ./expectedResults/$TEST_DIRECTORY/$TEST_NAME.xml

if $CREATE_EXE
then
    ./results/$TEST_DIRECTORY/$TEST_NAME.exe > ./results/$TEST_DIRECTORY/$TEST_NAME.out
    cmp -s ./results/$TEST_DIRECTORY/$TEST_NAME.out ./expectedResults/$TEST_DIRECTORY/$TEST_NAME.out
fi

exit $?

