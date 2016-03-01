#!/usr/bin/env bash

if [ `id -u` -ne 0 ]; then
    echo "Error: you must run this script as root"
    exit 1
fi

echo "AvanceDB may take a few seconds to stop..."

if [[ -d "/etc/systemd" && `systemctl --version` ]]; then
    pushd `dirname $0` > /dev/null
    systemd/uninstall.sh
    RETVAL=$?
    popd > /dev/null
    exit $RETVAL
else
    echo "Error: This script only supports systems running systemd"
    exit 2
fi
