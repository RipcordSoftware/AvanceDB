build: force_true
	cd src/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build

all: force_true
	cd src/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all

clean: force_true
	cd src/libscriptobject && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean
	cd src/avancedb && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean

force_true:
	true
