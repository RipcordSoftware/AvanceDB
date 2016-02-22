#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

if [ ! -d "/etc/systemd" ]; then
    echo "Error: This script only supports systems running systemd"
    exit 2
fi

mkdir -p /usr/local/avancedb && \
cp -Rf `dirname $0`/../src/avancedb/dist/Release/GNU-Linux-x86/* /usr/local/avancedb && \
cp -f `dirname $0`/systemd/avancedb.service /etc/systemd/system/ && \
systemctl daemon-reload && \
systemctl enable avancedb && \
systemctl start avancedb && \
systemctl status avancedb
