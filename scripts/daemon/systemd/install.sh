#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

cp -f `dirname $0`/avancedb.service /etc/systemd/system/ && \
systemctl daemon-reload && \
systemctl enable avancedb && \
systemctl start avancedb && \
systemctl status avancedb
