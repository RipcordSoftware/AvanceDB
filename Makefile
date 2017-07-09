SHELL := bash

GTEST_VER=1.7.0

.PHONY: build all clean docker .googletest .gperftools

.NOTPARALLEL: test ext_test

build all: .googletest .gperftools
	cd externals/libjsapi && $(MAKE) $@
	cd externals/libscriptobject && $(MAKE) $@
	cd externals/libhttpserver && $(MAKE) $@
	cd src/avancedb && $(MAKE) $(MFLAGS) $@

install:
	@pushd ./scripts && ./install.sh; popd

uninstall:
	@pushd ./scripts && ./uninstall.sh; popd

test: build ext_test
	cd src/avancedb && $(MAKE) $@

ext_test:
	cd src/avancedb/test/scripts/javascript/unit && ./test.sh
	cd src/avancedb/test/scripts/python && ./test.sh
	cd src/avancedb/test/scripts/ruby && ./test.sh

clean:
	cd externals/libjsapi && $(MAKE) $@
	cd externals/libscriptobject && $(MAKE) $@
	cd externals/libhttpserver && $(MAKE) $@
	cd src/avancedb && $(MAKE) $@

docker:
	cd docker && $(MAKE) build

.googletest:
	if [ "${CI}" = "true" ] && [ "${TRAVIS_OS_NAME}" = "linux" ]; then \
		curl ftp://ftp.ripcordsoftware.com/pub/gtest-${GTEST_VER}-travis-ci-externals-installed.tar.xz -O && \
		tar xfJ gtest-*; \
	elif [ ! -d externals/gtest-${GTEST_VER}/lib/.libs ]; then \
		mkdir -p externals && \
		cd externals && \
		if [ ! -f gtest-${GTEST_VER}.zip ]; then curl ftp://ftp.ripcordsoftware.com/pub/gtest-${GTEST_VER}.zip -O; fi && \
		unzip gtest-${GTEST_VER}.zip && \
		cd gtest-${GTEST_VER} && \
		./configure && \
		$(MAKE) && \
		if [ ! -d "../installed/include" ]; then mkdir -p ../installed/include; fi && \
		if [ ! -d "../installed/lib" ]; then mkdir -p ../installed/lib; fi && \
		cp -Rf include/* ../installed/include && \
		cp -Rf lib/.libs/* ../installed/lib; \
	fi

.gperftools:
	cd externals/gperftools && \
	if [ ! -f configure ]; then \
		./autogen.sh && \
		./configure --enable-minimal --with-tcmalloc-pagesize=32 --prefix=${PWD}/externals/installed && \
		$(MAKE) && \
		$(MAKE) install; \
	fi
