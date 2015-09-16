#!/bin/bash

npm --version &> /dev/null
if [ $? -eq 127 ]; then
    echo 'You should install nodejs & npm before running this test'
    exit 1
fi

netstat -an | grep tcp | grep -P 'couchdb|5984' &> /dev/null
if [ $? -ne 0 ]; then
    if [ "${CI}" == "" ]; then
        echo 'You should install and/or start couchdb before running this test'
        exit 2
    fi   
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

DELAY=2
if [ "${CI}" = "true" ]; then
    DELAY=10
fi

TEST_DIR=$PWD

pushd ../../../dist/${CONFIGURATION}/GNU-Linux-x86
./avancedb -p 15994 &> ${TEST_DIR}/avance_test.log  &
ADB_PID=$!
sleep ${DELAY}
popd

node_modules/mocha/bin/mocha -t `expr ${DELAY} \* 1000`
STATUS=$?
sleep ${DELAY}
kill $ADB_PID

exit $STATUS
