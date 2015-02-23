#!/bin/bash

V8_VER=3.31
LIBMICROHTTPD_VER=0.9.39

if [ ! -d externals/depot_tools ] || [ ! -d externals/couchdb ] || [ ! -d externals/libhttpserver ]; then
	git submodule init
	git submodule update
fi

if [ ! -d externals/v8 ]; then
	pushd externals
	export PATH=`pwd`/depot_tools:$PATH
	fetch v8
	cd v8
	git checkout branch-heads/${V8_VER}
	make native -j 4
	popd
fi

if [ ! -d externals/libmicrohttpdi-${LIBMICROHTTPD_VER} ]; then
	pushd externals
	curl ftp://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-${LIBMICROHTTPD_VER}.tar.gz -O
	tar xvfz libmicrohttpd-${LIBMICROHTTPD_VER}.tar.gz
	cd libmicrohttpd-${LIBMICROHTTPD_VER}
	./configure
	make -j 4	
	popd
fi

if [ ! -d externals/libhttpserver/build ]; then
	pushd externals/libhttpserver
	./bootstrap
	mkdir build
	cd build
	export LIBMICROHTTPD_CFLAGS=-I`pwd`/../../libmicrohttpd-${LIBMICROHTTPD_VER}/src/include/
	export LIBMICROHTTPD_LIBS=-L`pwd`/../../libmicrohttpd-${LIBMICROHTTPD_VER}/src/microhttpd/.libs/
	../configure --enable-cpp11
	make -j 4
	popd
fi
