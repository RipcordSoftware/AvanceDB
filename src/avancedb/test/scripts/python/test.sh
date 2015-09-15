#!/bin/bash

python --version &> /dev/null
if [ $? -eq 127 ]; then
    echo 'You should install python before running this test'
    exit 1
fi

if [ ! -d "site-packages" ]; then
    pip install -r requirements.txt -t site-packages
    if [ $? -ne 0 ]; then
        exit $?
    fi
fi

CONFIGURATION=$1
if [ "$CONFIGURATION" == "" ]; then
    CONFIGURATION=Debug
fi

DELAY=2
if [ "${CI}" = "true" ]; then
    DELAY=10
fi

TEST_DIR=$PWD

pushd ../../../dist/${CONFIGURATION}/GNU-Linux-x86
./avancedb -p 15996 &> ${TEST_DIR}/avance_test.log  &
ADB_PID=$!
sleep ${DELAY}
popd

PYTHONPATH=$PYTHONPATH:site-packages python test.py
STATUS=$?
sleep ${DELAY}
kill $ADB_PID

exit $STATUS

