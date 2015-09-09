#!/bin/bash

whereis ruby > /dev/null
if [ $? -ne 0 ]; then
    echo 'You must install Ruby first'
    exit 1
fi

whereis gem > /dev/null
if [ $? -ne 0 ]; then
    echo 'You must install Ruby Gems first'
    exit 2
fi

whereis ruby.h > /dev/null
if [ $? -ne 0 ]; then
    echo 'You must install Ruby dev/develop first'
    exit 3
fi

# add the gems executable dir to the path
export PATH=${PATH}:`ruby -rubygems -e 'puts Gem.user_dir'`/bin

gem list | grep bundler > /dev/null
if [ $? -ne 0 ]; then
    gem install --user-install bundler
fi

gem list | grep rake > /dev/null
if [ $? -ne 0 ]; then
    gem install --user-install rake
fi

gem list | grep rspec > /dev/null
if [ $? -ne 0 ]; then
    gem install --user-install rspec
fi

gem list | grep faker > /dev/null
if [ $? -ne 0 ]; then
    gem install --user-install faker
fi

gem list | grep airborne > /dev/null
if [ $? -ne 0 ]; then
    gem install --user-install airborne
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
./avancedb -p 15994 &> ${TEST_DIR}/avance_test.log &
ADB_PID=$!
sleep ${DELAY}
popd

rspec test_api.rspec
STATUS=$?
sleep ${DELAY}
kill $ADB_PID

exit $STATUS
