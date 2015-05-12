#!/bin/bash

#V8_VER=3.31

if #[ ! -d externals/depot_tools ] || \
   [ ! -d externals/couchdb ]; then
	git submodule init
	git submodule update
fi

#if [ ! -d externals/v8 ]; then
#	pushd externals
#	export PATH=`pwd`/depot_tools:$PATH
#	fetch v8
#	cd v8
#	git checkout branch-heads/${V8_VER}
#	make native -j 4
#	popd
#fi
