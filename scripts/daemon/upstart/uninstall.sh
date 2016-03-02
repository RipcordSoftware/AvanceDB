#!/usr/bin/env bash

if [[ `id -u` -ne 0 ]]; then
    echo "Error: you must run this script as root"
    exit 1
fi

service avancedb stop && \
update-rc.d -f avancedb remove && \
rm -f /etc/init.d/avancedb
