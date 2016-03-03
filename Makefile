SHELL := /usr/bin/env bash

GTEST_VER=1.7.0

build: force_true .googletest .gperftools
	cd externals/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd externals/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd externals/libhttpserver && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build

all: force_true .googletest .gperftools
	cd externals/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd externals/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd externals/libhttpserver && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all

install:
	@pushd ./scripts; ./install.sh; popd

uninstall:
	@pushd ./scripts; ./uninstall.sh; popd

test: build ext_test
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) test

.NOTPARALLEL: ext_test
ext_test:
	cd src/avancedb/test/scripts/javascript/unit && ./test.sh
	cd src/avancedb/test/scripts/python && ./test.sh
	cd src/avancedb/test/scripts/ruby && ./test.sh

clean: force_true
	cd externals/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd externals/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd externals/libhttpserver && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean

.googletest: force_true
	if [ "${CI}" = "true" ]; then \
		curl ftp://ftp.ripcordsoftware.com/pub/gtest-${GTEST_VER}-travis-ci-externals-installed.tar.xz -O && \
		tar xfJ gtest-*; \
	elif [ ! -d externals/gtest-${GTEST_VER}/lib/.libs ]; then \
		mkdir -p externals && \
		cd externals && \
		if [ ! -f gtest-${GTEST_VER}.zip ]; then curl https://googletest.googlecode.com/files/gtest-${GTEST_VER}.zip -O; fi && \
		unzip gtest-${GTEST_VER}.zip && \
		cd gtest-${GTEST_VER} && \
		./configure && \
		$(MAKE) -j 2 && \
		if [ ! -d "../installed/include" ]; then mkdir -p ../installed/include; fi && \
		if [ ! -d "../installed/lib" ]; then mkdir -p ../installed/lib; fi && \
		cp -Rf include/* ../installed/include && \
		cp -Rf lib/.libs/* ../installed/lib; \
	fi

.gperftools: force_true
	cd externals/gperftools && \
	if [ ! -f configure ]; then \
		./autogen.sh && \
		./configure --enable-minimal --with-tcmalloc-pagesize=32 --prefix=${PWD}/externals/installed && \
		$(MAKE) -j 2 && \
		$(MAKE) install; \
	fi

force_true:
	true
