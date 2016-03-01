#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

grep avancedb /etc/passwd > /dev/null
if [ $? -ne 0 ]; then
    adduser --system --no-create-home avancedb
fi

mkdir -p /usr/local/avancedb && \
cp -Rf `dirname $0`/../src/avancedb/dist/Release/GNU-Linux-x86/* /usr/local/avancedb && \
daemon/install.sh
