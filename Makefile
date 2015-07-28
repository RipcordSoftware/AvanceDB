build: force_true
	cd externals/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd externals/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd externals/libhttpserver && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build

all: force_true
	cd externals/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd externals/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd externals/libhttpserver && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all

test: build
	cd src/avancedb/test/scripts/javascript && ./test.sh

clean: force_true
	cd externals/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd externals/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd externals/libhttpserver && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean

force_true:
	true
