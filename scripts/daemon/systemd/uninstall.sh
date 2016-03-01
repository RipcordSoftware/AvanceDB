#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

systemctl stop avancedb && \
systemctl disable avancedb && \
rm -f /etc/systemd/system/avancedb.service && \
systemctl daemon-reload
