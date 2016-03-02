#!/bin/bash

npm --version &> /dev/null
if [ $? -eq 127 ]; then
    echo 'You should install nodejs & npm before running this test'
    exit 1
fi

netstat -an | grep tcp | grep -E 'couchdb|5984' &> /dev/null
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

APPROOT=../../../../dist/${CONFIGURATION}/GNU-Linux-x86
${APPROOT}/avancedb --daemon -p 15994 --dir ${APPROOT} -o avancedb_test.log -e avancedb_test.err --pid avancedb_test.pid
sleep ${DELAY}

node_modules/mocha/bin/mocha -t `expr ${DELAY} \* 1000`
STATUS=$?
sleep ${DELAY}
kill `cat avancedb_test.pid`

exit $STATUS
