FROM ubuntu:16.04
MAINTAINER Craig Minihan <craig@ripcordsoftware.com>
USER root

# update apt and install basic tools
RUN apt-get update -qq && \
    apt-get install -y -qq apt-utils sudo curl wget unzip bzip2 xz-utils pkg-config rsync net-tools telnet nano

# C++ tools
RUN apt-get install -y -qq git gcc g++ make autoconf autoconf2.13 automake libtool ccache

# C++ libraries
RUN apt-get install -y -qq zlib1g-dev libboost-all-dev libcppunit-dev

# add an avancedb user to build under
RUN adduser --disabled-password --gecos "" avancedb
