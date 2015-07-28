#!/bin/bash

which lcov > /dev/null
if [ $? -ne 0 ]; then
   echo "You must have lcov installed to run this script."
   echo "Install with 'yum install lcov' or 'apt-get install lcov'"
   exit 1
fi

lcov -b src/avancedb --directory src/avancedb/build/Debug/GNU-Linux-x86/ --capture --output-file coverage.info
lcov --remove coverage.info 'tests/*' '/usr/*' '*/externals/*' --output-file coverage.info
lcov --list coverage.info
if [ "$CI" = "true" ] && [ "$TRAVIS" = "true" ]; then
    coveralls-lcov coverage.info
else
    rm -rf coverage
    mkdir -p coverage
    cd coverage
    genhtml ../coverage.info
    cd ..
fi
