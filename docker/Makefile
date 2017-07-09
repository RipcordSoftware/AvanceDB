# A makefile script to build a runnable AvanceDB container
#
# Should be run from inside a git repo on a machine with Docker installed and working.
# 
# Overview:
# 1) creates a development image containing all the build tools we'll need to build AvanceDB (dev-img)
# 2) creates a buildable image, based on the development image, containing the source code (build-img)
# 3) runs the buildable image building AvanceDB with the development tools and places the build output in the 'output' directory
# 4) creates the runnable image (run-img)
#
# Versioning:
# all images except the dev image contain the commit hash in the image name
# the runnable image can be versioned based on the version number in the current branch (eg. release-1.2.3 would be avancedb:1.2.3)
# the runnable image can be set to avancedb:latest

SHELL:=bash

# limit the number of parallel source build jobs
BUILD_JOBS?=2

# we don't want parallel building of docker images
.NOTPARALLEL:

# our targets are all phonies
.PHONY: all help build run stop version clean purge purge-all promote .build-dirs dev-img build-img run-img

GIT_DIR:=$(shell git rev-parse --show-toplevel)
COMMIT:=$(shell git log --format=format:"%h" -1)
VERSION:=$(shell git branch | grep -e "\*\s" | grep -e "[0-9]\+\.[0-9]\+\.[0-9]\+" -o)

WORKING_DIR:=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR:="${WORKING_DIR}/build/${COMMIT}"
CCACHE_DIR:="${BUILD_DIR}/.ccache"
OUTPUT_DIR:="${BUILD_DIR}/output"
CLONE_DIR:="${BUILD_DIR}/clone"
EMPTY_DIR:="${BUILD_DIR}/empty"
DOWNLOADS_DIR:="${BUILD_DIR}/Downloads"

DISTRO:=ubuntu
DISTRO_VER:="16.04"
IMAGE_PREFIX:="ripcordsoftware/${DISTRO}-avancedb"
DEV_IMAGE:="${IMAGE_PREFIX}-dev:${DISTRO_VER}"
BUILD_IMAGE:="${IMAGE_PREFIX}-build-${COMMIT}:${DISTRO_VER}"
RUN_IMAGE:="${IMAGE_PREFIX}-${COMMIT}"

all help:
	@echo "command line:"
	@echo " build     : run the build process"
	@echo " run       : run the built application"
	@echo " stop      : stop the running application"
	@echo " version   : tag the built image with the branch version"
	@echo " promote   : tag the built image as latest"
	@echo " clean     : removes any running containers and the runnable image"
	@echo " purge     : removes all running containers, the runnable image and the build image"
	@echo " purge-all : removes all running containers, all images and build artifacts"

build: run-img

run-img: build-img
	# actually build avancedb now
	if [ -z "`docker images -f "reference=${RUN_IMAGE}" -q`" ]; then \
		cd "${WORKING_DIR}" && \
		docker run -t --rm \
			-v "${CCACHE_DIR}:/home/avancedb/.ccache" \
			-v "${OUTPUT_DIR}:/home/avancedb/output" \
			-v "${DOWNLOADS_DIR}:/home/avancedb/Downloads" \
			"${BUILD_IMAGE}" \
			/bin/su avancedb -c \
			"export SHELL=/bin/bash && cd ~/AvanceDB && make -j${BUILD_JOBS} CONF=Release CC='ccache gcc' CXX='ccache g++' && cp -Rf src/avancedb/dist ~/output && find . -regex '.*?externals/[a-z0-9\.\-]+\(zip\|bz2\)' -regextype posix-basic -exec cp -f {} ~/Downloads \;" - && \
		# create the runnable image; \
		cp -f Dockerfile.${DISTRO}-avancedb-run "${OUTPUT_DIR}/Dockerfile" && \
		docker build -t "${RUN_IMAGE}" --build-arg "BIN_SOURCE=dist/Release/GNU-Linux-x86" "${OUTPUT_DIR}"; \
	fi

dev-img: .build-dirs
	# build the baseline build image
	if [ -z "`docker images -f "reference=${DEV_IMAGE}" -q`" ]; then \
		cd "${WORKING_DIR}" && \
		cp -f Dockerfile.${DISTRO}-avancedb-dev "${EMPTY_DIR}/Dockerfile" && \
		docker build -t "${DEV_IMAGE}" "${EMPTY_DIR}"; \
	fi

build-img: dev-img
	# create a development image with the cloned source
	if [ -z "`docker images -f "reference=${BUILD_IMAGE}" -q`" ]; then \
		cd "${WORKING_DIR}" && \
		if [ ! -d "${CLONE_DIR}/AvanceDB" ]; then git clone --recursive "${GIT_DIR}" "${CLONE_DIR}/AvanceDB"; fi && \
		cp -f Dockerfile.${DISTRO}-avancedb-build "${CLONE_DIR}/Dockerfile" && \
		docker build -t "${BUILD_IMAGE}" "${CLONE_DIR}"; \
	fi

.build-dirs:
	# create directories for transient and output data	
	mkdir -p "${BUILD_DIR}" && \
	mkdir -p "${CCACHE_DIR}" && \
	mkdir -p "${OUTPUT_DIR}" && \
	mkdir -p "${EMPTY_DIR}" && \
	mkdir -p "${CLONE_DIR}" && \
	mkdir -p "${DOWNLOADS_DIR}"

run:
	# run the runnable image
	docker run -d -p 5994:5994 "${RUN_IMAGE}"

stop:
	# stop the running image
	$(shell docker stop $(docker ps -a -q -f ancestor="${RUN_IMAGE}"))

version:
	# tag the run image with the version of the current branch
	@if [ -z "${VERSION}" ]; then \
		echo "Error: can't promote an image from a non-versioned branch"; \
		exit 4; \
	fi
	-docker rmi "avancedb:${VERSION}"
	docker tag "${RUN_IMAGE}" "avancedb:${VERSION}"

clean:
	# remove the run container image
	-docker stop `docker ps -a -q -f ancestor=${RUN_IMAGE}` &> /dev/null
	-docker rm `docker ps -a -q -f ancestor=${RUN_IMAGE}` &> /dev/null
	-docker rmi "${RUN_IMAGE}" &> /dev/null

purge: clean
	# remove the source container image
	-docker stop `docker ps -a -q -f ancestor=${BUILD_IMAGE}` &> /dev/null
	-docker rm `docker ps -a -q -f ancestor=${BUILD_IMAGE}` &> /dev/null
	-docker rmi "${BUILD_IMAGE}" &> /dev/null
	
purge-all: purge
	# remove the dev image and any build artifacts
	-docker stop `docker ps -a -q -f ancestor=${DEV_IMAGE}` &> /dev/null
	-docker rm `docker ps -a -q -f ancestor=${DEV_IMAGE}` &> /dev/null
	-docker rmi "${DEV_IMAGE}" &> /dev/null
	-rm -rf "${BUILD_DIR}"

promote:
	# promote the run image to be the latest
	-docker rmi "avancedb:latest"
	docker tag "${RUN_IMAGE}" "avancedb:latest"

