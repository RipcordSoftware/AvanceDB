#!/usr/bin/env bash

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

USER_INSTALL=--user-install
if [ -f ~/.rvm/scripts/rvm ]; then
    unset USER_INSTALL
fi

gem list | grep bundler > /dev/null
if [ $? -ne 0 ]; then
    gem install ${USER_INSTALL} bundler
fi

if [ $? -ne 0 ]; then
    echo 'Failed to install bundler'
    exit 4
fi

if [ $EUID -ne 0 ] && [ "$GEM_HOME" = "" ]; then
    bundle install --verbose --path=~/.gem
else
    bundle install --verbose
fi

if [ $? -ne 0 ]; then
    echo 'Bundler failed to install the gems'
    exit 5
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
./avancedb --daemon -p 15995 -o "${TEST_DIR}/avancedb_test.log" -e "${TEST_DIR}/avancedb_test.err" --pid "${TEST_DIR}/avancedb_test.pid"
sleep ${DELAY}
popd

rspec test_api.rspec
STATUS=$?
sleep ${DELAY}
kill `cat avancedb_test.pid`

exit $STATUS
