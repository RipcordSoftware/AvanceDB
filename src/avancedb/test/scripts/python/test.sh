#!/usr/bin/env bash

PYTHON_BIN=`which python-2.7 || which python2.7 || which python27 || which python`

if [ "$PYTHON_BIN" = "" ]; then
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
if [ "${CI}" = "true" ] || [ "$JENKINS_HOME" != "" ] ; then
    DELAY=10
fi

TEST_DIR=$PWD

pushd ../../../dist/${CONFIGURATION}/GNU-Linux-x86
./avancedb --daemon -p 15996 -o "${TEST_DIR}/avancedb_test.log" -e "${TEST_DIR}/avancedb_test.err" --pid "${TEST_DIR}/avancedb_test.pid"
sleep ${DELAY}
popd

PYTHONPATH=$PYTHONPATH:site-packages $PYTHON_BIN test.py
STATUS=$?
sleep ${DELAY}
kill `cat avancedb_test.pid`

exit $STATUS

