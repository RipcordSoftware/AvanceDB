[![Build Status](https://travis-ci.org/RipcordSoftware/AvanceDB.svg)](https://travis-ci.org/RipcordSoftware/AvanceDB)
[![Coverage Status](https://coveralls.io/repos/RipcordSoftware/AvanceDB/badge.svg?branch=master&service=github)](https://coveralls.io/github/RipcordSoftware/AvanceDB?branch=master)
[![License](http://img.shields.io/:license-agpl-blue.svg)](http://doge.agpl-license.org)
# AvanceDB
An in-memory database based on the CouchDB REST API and containing the CouchDB Futon and Fauxton web sites.

Written in C++ 11 with Boost and the following technologies:
* [Mozilla SpiderMonkey JSAPI 31](https://github.com/mozilla/gecko-dev/tree/esr31/js/src)
* [RipcordSoftware/libhttpserver](http://github.com/RipcordSoftware/libhttpserver)
* [RipcordSoftware/libjsapi](http://github.com/RipcordSoftware/libjsapi)
* [RipcordSoftware/libscriptobject](http://github.com/RipcordSoftware/libscriptobject)
* [RipcordSoftware/lazyflatset](https://github.com/RipcordSoftware/lazyflatset)
* [vivkin/gason](https://github.com/vivkin/gason)

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
