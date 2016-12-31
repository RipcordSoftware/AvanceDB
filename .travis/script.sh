# detect a missing ccache alias for the compiler we want
CCACHE_BIN_PATH=`type -p ccache`
if [ -n "${CCACHE_BIN_PATH}" ]; then
    echo "Found ccache at: ${CCACHE_BIN_PATH}"
    CCACHE_LIB_PATH=$(dirname `type -p g++ || type -p clang++` | grep ccache)
    echo "Found ccache lib at: ${CCACHE_LIB_PATH}"
    test ! -e "${CCACHE_LIB_PATH}/${_CC}" && export _CC="ccache ${_CC}"
    test ! -e "${CCACHE_LIB_PATH}/${_CXX}" && export _CXX="ccache ${_CXX}"
fi


# export CC, CXX and LDFLAGS into the environment
export CC="${_CC}"
export CXX="${_CXX}"
export LDFLAGS="${_LDFLAGS}"

# report what we are using
echo *******************************************************************
echo CC=${CC}
echo CXX=${CXX}
echo LDFLAGS=${LDFLAGS}
echo *******************************************************************

# build
make CC="${_CC}" CXX="${_CXX}" LDFLAGS="${_LDFLAGS}" -j2 all && make CC="${_CC}" CXX="${_CXX}" LDFLAGS="${_LDFLAGS}" test || exit $?

# generate coverage
if [ "${_COV}" != "" ]; then
    ./coverage.sh "${_COV}"
fi
