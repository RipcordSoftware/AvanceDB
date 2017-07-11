# Building AvanceDB Containers
On any platform where Docker can run Linux containers you can build AvanceDB following the steps below.

## Background
Building a runnable AvanceDB container requires three container build stages, these are:

* dev-img: a stock Ubuntu 16.04 image is built with all the development tools needed to build AvanceDB
* build-img: AvanceDB is cloned into this image which is based on `dev-img`, the build is then executed
* run-img: the output of the build stage is injected onto a stock Ubuntu 16.04 with a minimal set of packages capable of running AvanceDB


## Directory Structure
Container builds happen under the `git` root in `docker/build`. For each commit a directory is created 
storing intermediate build state and output files.

> e.g. for the commit fdc5448 the directory `builds/fdc5448` will be created.

In the state directory the following directories are created:

* .ccache - stores ccache build files which can greatly speed up repeated builds
* clone - the build process clones the AvanceDB source into this directory before injecting it into the build image
* empty - an empty directory used when the image build needs no context files
* output - if the build completes successfully will contain the output binaries and assets
* Downloads - the build will store temporary downloaded files here, this speeds up repeated builds

All of these directories can be removed and the build re-executed if required.

## Building
The container build process depends on `GNU Make`, `git` and `Docker`. You must have all these tools installed prior to attempting to build the container.

To build the container run:
```bash
make build
```

The build process will create the dev, build and runnable images. After the build is completed running `$ docker images` should yield something like:

|REPOSITORY                                      |TAG                 |IMAGE ID            |CREATED             |SIZE
|------------------------------------------------|:------------------:|--------------------|--------------------|----------
|ripcordsoftware/ubuntu-avancedb-build-fdc5448   |16.04               |6ead84e8d11c        |2 days ago          |947MB
|ripcordsoftware/ubuntu-avancedb-fdc5448         |latest              |ad1d36e1bc09        |2 days ago          |354MB
|ripcordsoftware/ubuntu-avancedb-dev             |16.04               |bf85e293bce1        |5 days ago          |739MB


The build and runnable images contain the commit id in their name, the dev image can be shared between commits, so it doesn't have a commit in it's name.

To run the AvanceDB image based on the image names above: `docker run -d -p 5994:5994 ripcordsoftware/ubuntu-avancedb-fdc5448` or alternately just `make run`.
You can use `make stop` to stop the running image.

## Cleaning out and building again
The `Makefile` supports the following options:

* clean - removes the runnable image associated with the current commit, run `make build` again to rebuild it
* purge - removes the runnable and build images
* purge-all - removes the runnable, build and dev images

## Versioning/Tagging
The `Makefile` supports the `version` and `promote` build targets. 

Version will unpick a version number from the current git branch name and tag a runnable image corresponding to the current commit. 

For example:
`ripcordsoftware/ubuntu-avancedb-fdc5448` on branch `release-0.2.7` would become `ripcordsoftware/avancedb:0.2.7`

Promote takes the runnable image and makes it the latest:
`ripcordsoftware/ubuntu-avancedb-fdc5448` would become `ripcordsoftware/avancedb:latest`

These two images can now be pushed to the Docker Hub.
