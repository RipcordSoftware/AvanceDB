mkdir -p .travis.temp
pushd .travis.temp

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    ### install Boost
    wget https://github.com/Viq111/travis-container-packets/releases/download/boost-1.57.0/boost.tar.bz2
    tar -xjf boost.tar.bz2
    rm boost.tar.bz2
    export LIBRARY_PATH=$(pwd)/boost/lib:$LIBRARY_PATH
    export LD_LIBRARY_PATH=$(pwd)/boost/lib:$LD_LIBRARY_PATH
    export CPLUS_INCLUDE_PATH=$(pwd)/boost/include/:$CPLUS_INCLUDE_PATH
    ### install lcov and coveralls
    curl http://ftp.uk.debian.org/debian/pool/main/l/lcov/lcov_1.11.orig.tar.gz -O
    tar xfz lcov_1.11.orig.tar.gz
    mkdir -p lcov && make -C lcov-1.11/ install PREFIX=~/lcov
    export PATH=~/lcov/usr/bin:$PATH
    gem install coveralls-lcov
elif [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    brew update
    brew tap homebrew/versions
    brew install autoconf213
    brew unlink autoconf213
    brew install autoconf
    brew install automake
    brew install libtool
    brew install pkg-config
    brew install boost
    test -d ~/.ccache && brew install ccache

    # soft-link the boost libs so we can link to them
    BOOST_LIBS=/usr/local/lib
    ln -s ${BOOST_LIBS}/libboost_thread-mt.a ${BOOST_LIBS}/libboost_thread.a
    ln -s ${BOOST_LIBS}/libboost_thread-mt.dylib ${BOOST_LIBS}/libboost_thread.dylib
    ln -s ${BOOST_LIBS}/libboost_atomic-mt.a ${BOOST_LIBS}/libboost_atomic.a
    ln -s ${BOOST_LIBS}/libboost_atomic-mt.dylib ${BOOST_LIBS}/libboost_atomic.dylib
fi

popd
