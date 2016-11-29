make CC=${_CC} CXX=${_CXX} LDFLAGS=${_LDFLAGS} -j 2 all && make CC=${_CC} CXX=${_CXX} LDFLAGS=${_LDFLAGS} test || exit $?

if [ "${_COV}" != "" ]; then
    ./coverage.sh ${_COV}
fi
