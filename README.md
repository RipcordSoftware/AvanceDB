[![Build Status](https://travis-ci.org/RipcordSoftware/AvanceDB.svg)](https://travis-ci.org/RipcordSoftware/AvanceDB)
[![Coverage Status](https://coveralls.io/repos/RipcordSoftware/AvanceDB/badge.svg?branch=master&service=github)](https://coveralls.io/github/RipcordSoftware/AvanceDB?branch=master)
[![License](http://img.shields.io/:license-agpl-blue.svg)](http://doge.agpl-license.org)
[![Twitter URL](https://img.shields.io/twitter/url/http/shields.io.svg?style=social?style=plastic)](https://twitter.com/RipcordSoftware)
[![Join the chat at https://gitter.im/RipcordSoftware/AvanceDB](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/RipcordSoftware/AvanceDB?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# AvanceDB
An in-memory database implementing a large subset of the [CouchDB](http://couchdb.apache.org/) REST [API](http://docs.couchdb.org/). AvanceDB has blistering fast document lookup and map/reduce performance. If you are currently using CouchDB and struggle with view build times then AvanceDB should be a seamless replacement for your view workload.

We aim to support the following CouchDB features:
* Futon and Fauxton
* JavaScript Map/Reduce
* Replication
* Apache Lucene and ElasticSearch integration
* Compatibility with common CouchDB clients like Cradle, Nano and others
* Compatibility with PouchDB

AvanceDB is not designed to replace CouchDB for document storage. We have permanent document store functionality on the product roadmap but we are unlikely to realize this feature for quite a while.

The core is written in C++ 11 with Boost and map/reduce executed by an embedded SpiderMonkey JSAPI instance. We chose C++ because it is very fast and gives control over memory allocations that simply isn't available with managed languages like Java and C#.

AvanceDB will run on Linux on a humble Raspberry Pi 2 or an enterprise server. Via the magic of [Brew](http://brew.sh/) you can spin up an instance of AvanceDB on OSX too. If you are running Windows you are out of luck, at least for the moment.

## Build Status

| OS           | Architecture       | Status                       |
|--------------|--------------------|------------------------------|
| Debian 8.6   | amd64              | [![debian-8.6-amd64](https://jenkins.ripcordsoftware.com/buildStatus/icon?job=avancedb-debian-8.6)](https://jenkins.ripcordsoftware.com/job/avancedb-debian-8.6/) |
| Ubuntu 14.04 | amd64              | [![ubutu-14.04-amd64](https://jenkins.ripcordsoftware.com/buildStatus/icon?job=avancedb-ubuntu-14.04)](https://jenkins.ripcordsoftware.com/job/avancedb-ubuntu-14.04/) |
| Ubuntu 16.04 | amd64              | [![ubuntu-16.04-amd64](https://jenkins.ripcordsoftware.com/buildStatus/icon?job=avancedb-ubuntu-16.04)](https://jenkins.ripcordsoftware.com/job/avancedb-ubuntu-16.04/) |
| CentOS 6.8   | amd64              | [![centos-6.8-amd64](https://jenkins.ripcordsoftware.com/buildStatus/icon?job=avancedb-centos-6.8)](https://jenkins.ripcordsoftware.com/job/avancedb-centos-6.8/) |
| CentOS 7.3   | amd64              | [![centos-7.3-amd64](https://jenkins.ripcordsoftware.com/buildStatus/icon?job=avancedb-centos-7.3)](https://jenkins.ripcordsoftware.com/job/avancedb-centos-7.3/) |
| Fedora 25    | amd64              | [![fedora-25-amd64](https://jenkins.ripcordsoftware.com/buildStatus/icon?job=avancedb-fedora-25)](https://jenkins.ripcordsoftware.com/job/avancedb-fedora-25/) |

## Building
Check out our [Wiki](https://github.com/RipcordSoftware/AvanceDB/wiki) for information about building AvanceDB on your platform.

## API
AvanceDB is under active development, you can check out the current API support status [here](https://github.com/RipcordSoftware/AvanceDB/wiki/CouchDB-API-Support).

## Contributing
### C++
If you are a C++ 11 ace with interest in NoSQL and CouchDB specifically then we'd love any contributions you have. Chat to us on [Gitter](https://gitter.im/RipcordSoftware/AvanceDB) if you think you can help us out.

### Unit Tests
We aim for all major CouchDB clients like [Cradle](https://github.com/flatiron/cradle), [Nano](https://github.com/dscape/nano), [CouchDB-Python](https://github.com/djc/couchdb-python) to operate with AvanceDB. We need unit tests for these platforms, so if you have the language skills and the time and want to contribute then we'd love to hear from you. We hang around on [Gitter](https://gitter.im/RipcordSoftware/AvanceDB) all the time, come over for a chat.
