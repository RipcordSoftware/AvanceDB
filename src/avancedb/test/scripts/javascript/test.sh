#!/bin/bash

npm --version &> /dev/null
if [ $? -eq 127 ]; then
    echo 'You should install node/npm before running this test'
    exit 1
fi

if [ ! -d "node_modules" ]; then
    npm install
    if [ $? -ne 0 ]; then
        exit $?
    fi
fi

CONFIGURATION=$1
if [ "$CONFIGURATION" == "" ]; then
    CONFIGURATION=Debug
fi

TEST_DIR=$PWD

pushd ../../../dist/${CONFIGURATION}/GNU-Linux-x86
./avancedb -p 15994 &> ${TEST_DIR}/avance_test.log  &
ADB_PID=$!
sleep 2
popd

node_modules/mocha/bin/mocha
STATUS=$?
sleep 2
kill $ADB_PID
exit $STATUS
