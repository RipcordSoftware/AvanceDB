#!/bin/bash

npm --version &> /dev/null
if [ $? -eq 127 ]; then
    echo 'You should install nodejs & npm before running this test'
    exit 1
fi

ps -A | grep couchdb &> /dev/null
if [ $? -ne 0 ]; then
    if [ "${CI}" == "" ]; then
        echo 'You should install and/or start couchdb before running this test'
        exit 2
    fi
    
    systemctl --version &> /dev/null
    if [ $? -eq 0 ]; then
        sudo -n systemctl start couchdb
    else
        service --version &> /dev/null
        if [ $? -eq 0 ]; then
            sudo -n service couchdb start
        fi
    fi
    
    if [ $? -ne 0 ]; then
        echo 'Unable to start CouchDB'
        exit 3
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
