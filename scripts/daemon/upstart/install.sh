#!/usr/bin/env bash

if [[ `id -u` -ne 0 ]]; then
    echo "Error: you must run this script as root"
    exit 1
fi

cp -f `dirname $0`/avancedb /etc/init.d/ && \
update-rc.d avancedb defaults && \
service avancedb start && \
service avancedb status
