#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

daemon/uninstall.sh && \
rm -rf /usr/local/avancedb
