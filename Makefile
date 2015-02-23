all: force_true
	cd src/avancedb; $(MAKE) $(MFLAGS)

clean: force_true
	cd src/avancedb; $(MAKE) clean

force_true:
	true
