#!/usr/bin/env bash

if [[ `id -u` -ne 0 ]]; then
    echo "Error: you must run this script as root"
    exit 1
fi

PREFIX=/usr/local
LOGDIR=${PREFIX}/var/log/avancedb
SHAREDIR=${PREFIX}/share/avancedb
RUNDIR=/var/run/avancedb

grep avancedb /etc/passwd > /dev/null
if [[ $? -ne 0 ]]; then
    adduser --system --home ${SHAREDIR} --group avancedb
fi

mkdir -p ${LOGDIR} && \
mkdir -p ${SHAREDIR} && \
mkdir -p ${RUNDIR} && \
pushd `dirname $0`/../src/avancedb/dist/Release/GNU-Linux-x86 && \
cp -f avancedb /usr/local/bin/ && \
cp -Rf www ${SHAREDIR} && \
popd && \
chown -R avancedb:avancedb ${LOGDIR} && \
chown -R avancedb:avancedb ${SHAREDIR} && \
chown -R avancedb:avancedb ${RUNDIR} && \
daemon/install.sh
