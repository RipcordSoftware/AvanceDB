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
    brew install boost

    # rename the boost libs so we can link to them
    BOOST_LIBS=$(echo /usr/local/Cellar/boost/?.??.*/lib)
    ln -s ${BOOST_LIBS}/libboost_thread-mt.a ${BOOST_LIBS}/libboost_thread.a
    ln -s ${BOOST_LIBS}/libboost_thread-mt.dylib ${BOOST_LIBS}/libboost_thread.dylib    
fi

popd
