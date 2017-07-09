FROM ubuntu:16.04
MAINTAINER Craig Minihan <craig@ripcordsoftware.com>
ARG BIN_SOURCE
EXPOSE 5994
USER root

# add an avancedb user and change to that dir
RUN adduser --disabled-password --gecos "" avancedb
WORKDIR /home/avancedb/bin

# copy in the avancedb binary and assets
COPY ${BIN_SOURCE} /home/avancedb/bin/

# update apt and install required C++ libraries
RUN apt-get update -qq && \
    apt-get install -y -qq zlib1g && \
    apt-get install -y -qq `ldd avancedb | grep -e "libboost_[a-z1-9\_]*\.so\.[0-9]\+\.[0-9]\+\.[0-9]\+" -o | sed 's/_/-/g' | sed 's/\.so\.//g'`

# run avancedb when the container starts
USER avancedb
ENTRYPOINT [ "./avancedb" ]
