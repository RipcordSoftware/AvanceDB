#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

if [ ! -d "/etc/systemd" ]; then
    echo "Error: This script only supports systems running systemd"
    exit 2
fi

echo "AvanceDB may take a few seconds to stop..."

systemctl stop avancedb && \
systemctl disable avancedb && \
rm -f /etc/systemd/system/avancedb.service && \
systemctl daemon-reload
