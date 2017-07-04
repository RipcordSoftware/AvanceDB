FROM debian:jessie

RUN apt-get update
RUN apt-get install -y curl wget unzip bzip2 pkgconf
RUN apt-get install -y g++ make autoconf autoconf2.13 automake libtool git libboost-all-dev zlib1g-dev

ADD LICENSE Makefile README.md /AvanceDB/
ADD externals/ /AvanceDB/externals/
ADD scripts/ /AvanceDB/scripts/
ADD src/ /AvanceDB/src/

WORKDIR /AvanceDB

RUN SHELL=/bin/bash make -j2 CONF=Release

EXPOSE 5994

WORKDIR /AvanceDB/src/avancedb/dist/Release/GNU-Linux-x86
ENTRYPOINT ./avancedb

