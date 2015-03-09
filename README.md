# AvanceDB
An in-memory database based on the CouchDB REST API and containing the CouchDB Futon and Fauxton web sites.

Written in C++ 11 with Boost and the following technologies:
* Google V8
* RipcordSoftware/libhttpserver

## Building
To build you'll need:
* GCC 4.8.x
* GNU Make
* Boost 1.53
* pthreads
* zlib

Steps:
* git clone ...
* run `submodules.sh`
* run `make`
* The binaries will appear under here: `src/avancedb/dist/`

We are currently building on CentOS 7. You may be successful on other systems - if you are let us know. For development we use NetBeans 8.0.2.
