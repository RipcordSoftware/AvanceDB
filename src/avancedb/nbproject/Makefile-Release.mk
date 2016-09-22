#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/527039428/base64.o \
	${OBJECTDIR}/_ext/1383664149/city.o \
	${OBJECTDIR}/_ext/1845599792/worker.o \
	${OBJECTDIR}/base64_helper.o \
	${OBJECTDIR}/config.o \
	${OBJECTDIR}/daemon.o \
	${OBJECTDIR}/database.o \
	${OBJECTDIR}/databases.o \
	${OBJECTDIR}/document.o \
	${OBJECTDIR}/document_attachment.o \
	${OBJECTDIR}/document_collection.o \
	${OBJECTDIR}/document_collection_results.o \
	${OBJECTDIR}/document_revision.o \
	${OBJECTDIR}/documents.o \
	${OBJECTDIR}/get_all_documents_options.o \
	${OBJECTDIR}/get_view_options.o \
	${OBJECTDIR}/http_server.o \
	${OBJECTDIR}/http_server_log.o \
	${OBJECTDIR}/json_stream.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/map_reduce.o \
	${OBJECTDIR}/map_reduce_query_key.o \
	${OBJECTDIR}/map_reduce_result.o \
	${OBJECTDIR}/map_reduce_result_array.o \
	${OBJECTDIR}/map_reduce_result_comparers.o \
	${OBJECTDIR}/map_reduce_results.o \
	${OBJECTDIR}/map_reduce_results_iterator.o \
	${OBJECTDIR}/map_reduce_shard_results.o \
	${OBJECTDIR}/map_reduce_thread_pool.o \
	${OBJECTDIR}/post_all_documents_options.o \
	${OBJECTDIR}/rest_config.o \
	${OBJECTDIR}/rest_exceptions.o \
	${OBJECTDIR}/rest_server.o \
	${OBJECTDIR}/script_array_jsapi_key_value_source.o \
	${OBJECTDIR}/script_array_jsapi_source.o \
	${OBJECTDIR}/script_object_jsapi_source.o \
	${OBJECTDIR}/script_object_response_stream.o \
	${OBJECTDIR}/set_thread_name.o \
	${OBJECTDIR}/uuid_helper.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f4

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../../externals/libhttpserver/src/libhttpserver/dist/Release/GNU-Linux-x86/libhttpserver.a ../../externals/libscriptobject/src/libscriptobject_gason/dist/Release/GNU-Linux-x86/libscriptobject_gason.a ../../externals/libscriptobject/src/libscriptobject/dist/Release/GNU-Linux-x86/libscriptobject.a ../../externals/libjsapi/src/libjsapi/dist/Release/GNU-Linux-x86/libjsapi.a ../../externals/libscriptobject/src/libscriptobject_msgpack/dist/Release/GNU-Linux-x86/libscriptobject_msgpack.a ../../externals/libjsapi/externals/installed/lib/libjs_static.ajs -lboost_regex -lboost_filesystem -lboost_program_options -lboost_thread -lboost_date_time -lboost_chrono -lboost_atomic -lboost_system -lpthread -lz $(LDLIBS)

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libhttpserver/src/libhttpserver/dist/Release/GNU-Linux-x86/libhttpserver.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject_gason/dist/Release/GNU-Linux-x86/libscriptobject_gason.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject/dist/Release/GNU-Linux-x86/libscriptobject.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libjsapi/src/libjsapi/dist/Release/GNU-Linux-x86/libjsapi.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject_msgpack/dist/Release/GNU-Linux-x86/libscriptobject_msgpack.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libjsapi/externals/installed/lib/libjs_static.ajs

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/527039428/base64.o: ../../externals/ConstTimeEncoding/base64.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/527039428
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/527039428/base64.o ../../externals/ConstTimeEncoding/base64.cpp

${OBJECTDIR}/_ext/1383664149/city.o: ../../externals/cityhash/src/city.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1383664149
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1383664149/city.o ../../externals/cityhash/src/city.cc

${OBJECTDIR}/_ext/1845599792/worker.o: ../../externals/thread-pool-cpp/thread_pool/worker.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1845599792
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1845599792/worker.o ../../externals/thread-pool-cpp/thread_pool/worker.cpp

${OBJECTDIR}/base64_helper.o: base64_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/base64_helper.o base64_helper.cpp

${OBJECTDIR}/config.o: config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/config.o config.cpp

${OBJECTDIR}/daemon.o: daemon.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/daemon.o daemon.cpp

${OBJECTDIR}/database.o: database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/database.o database.cpp

${OBJECTDIR}/databases.o: databases.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/databases.o databases.cpp

${OBJECTDIR}/document.o: document.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document.o document.cpp

${OBJECTDIR}/document_attachment.o: document_attachment.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_attachment.o document_attachment.cpp

${OBJECTDIR}/document_collection.o: document_collection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_collection.o document_collection.cpp

${OBJECTDIR}/document_collection_results.o: document_collection_results.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_collection_results.o document_collection_results.cpp

${OBJECTDIR}/document_revision.o: document_revision.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_revision.o document_revision.cpp

${OBJECTDIR}/documents.o: documents.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/documents.o documents.cpp

${OBJECTDIR}/get_all_documents_options.o: get_all_documents_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/get_all_documents_options.o get_all_documents_options.cpp

${OBJECTDIR}/get_view_options.o: get_view_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/get_view_options.o get_view_options.cpp

${OBJECTDIR}/http_server.o: http_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server.o http_server.cpp

${OBJECTDIR}/http_server_log.o: http_server_log.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server_log.o http_server_log.cpp

${OBJECTDIR}/json_stream.o: json_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/json_stream.o json_stream.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/map_reduce.o: map_reduce.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce.o map_reduce.cpp

${OBJECTDIR}/map_reduce_query_key.o: map_reduce_query_key.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_query_key.o map_reduce_query_key.cpp

${OBJECTDIR}/map_reduce_result.o: map_reduce_result.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_result.o map_reduce_result.cpp

${OBJECTDIR}/map_reduce_result_array.o: map_reduce_result_array.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_result_array.o map_reduce_result_array.cpp

${OBJECTDIR}/map_reduce_result_comparers.o: map_reduce_result_comparers.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_result_comparers.o map_reduce_result_comparers.cpp

${OBJECTDIR}/map_reduce_results.o: map_reduce_results.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_results.o map_reduce_results.cpp

${OBJECTDIR}/map_reduce_results_iterator.o: map_reduce_results_iterator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_results_iterator.o map_reduce_results_iterator.cpp

${OBJECTDIR}/map_reduce_shard_results.o: map_reduce_shard_results.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_shard_results.o map_reduce_shard_results.cpp

${OBJECTDIR}/map_reduce_thread_pool.o: map_reduce_thread_pool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_thread_pool.o map_reduce_thread_pool.cpp

${OBJECTDIR}/post_all_documents_options.o: post_all_documents_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/post_all_documents_options.o post_all_documents_options.cpp

${OBJECTDIR}/rest_config.o: rest_config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_config.o rest_config.cpp

${OBJECTDIR}/rest_exceptions.o: rest_exceptions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_exceptions.o rest_exceptions.cpp

${OBJECTDIR}/rest_server.o: rest_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_server.o rest_server.cpp

${OBJECTDIR}/script_array_jsapi_key_value_source.o: script_array_jsapi_key_value_source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_array_jsapi_key_value_source.o script_array_jsapi_key_value_source.cpp

${OBJECTDIR}/script_array_jsapi_source.o: script_array_jsapi_source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_array_jsapi_source.o script_array_jsapi_source.cpp

${OBJECTDIR}/script_object_jsapi_source.o: script_object_jsapi_source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_object_jsapi_source.o script_object_jsapi_source.cpp

${OBJECTDIR}/script_object_response_stream.o: script_object_response_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_object_response_stream.o script_object_response_stream.cpp

${OBJECTDIR}/set_thread_name.o: set_thread_name.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/set_thread_name.o set_thread_name.cpp

${OBJECTDIR}/uuid_helper.o: uuid_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/uuid_helper.o uuid_helper.cpp

# Subprojects
.build-subprojects:
	cd ../../externals/libhttpserver/src/libhttpserver && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libscriptobject/src/libscriptobject_gason && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libscriptobject/src/libscriptobject && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libjsapi/src/libjsapi && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libscriptobject/src/libscriptobject_msgpack && ${MAKE}  -f Makefile CONF=Release

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/basic_database_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} ../../externals/installed/lib/libgtest_main.a ../../externals/installed/lib/libgtest.a  -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/json_helper_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} ../../externals/installed/lib/libgtest_main.a ../../externals/installed/lib/libgtest.a  -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/map_reduce_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} ../../externals/installed/lib/libgtest_main.a ../../externals/installed/lib/libgtest.a  -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/basic_database_tests.o: tests/basic_database_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -I../../externals/installed/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/basic_database_tests.o tests/basic_database_tests.cpp


${TESTDIR}/tests/json_helper_tests.o: tests/json_helper_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -I../../externals/installed/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/json_helper_tests.o tests/json_helper_tests.cpp


${TESTDIR}/tests/map_reduce_tests.o: tests/map_reduce_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -I../../externals/installed/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/map_reduce_tests.o tests/map_reduce_tests.cpp


${OBJECTDIR}/_ext/527039428/base64_nomain.o: ${OBJECTDIR}/_ext/527039428/base64.o ../../externals/ConstTimeEncoding/base64.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/527039428
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/527039428/base64.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/527039428/base64_nomain.o ../../externals/ConstTimeEncoding/base64.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/527039428/base64.o ${OBJECTDIR}/_ext/527039428/base64_nomain.o;\
	fi

${OBJECTDIR}/_ext/1383664149/city_nomain.o: ${OBJECTDIR}/_ext/1383664149/city.o ../../externals/cityhash/src/city.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1383664149
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1383664149/city.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1383664149/city_nomain.o ../../externals/cityhash/src/city.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1383664149/city.o ${OBJECTDIR}/_ext/1383664149/city_nomain.o;\
	fi

${OBJECTDIR}/_ext/1845599792/worker_nomain.o: ${OBJECTDIR}/_ext/1845599792/worker.o ../../externals/thread-pool-cpp/thread_pool/worker.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1845599792
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1845599792/worker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1845599792/worker_nomain.o ../../externals/thread-pool-cpp/thread_pool/worker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1845599792/worker.o ${OBJECTDIR}/_ext/1845599792/worker_nomain.o;\
	fi

${OBJECTDIR}/base64_helper_nomain.o: ${OBJECTDIR}/base64_helper.o base64_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/base64_helper.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/base64_helper_nomain.o base64_helper.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/base64_helper.o ${OBJECTDIR}/base64_helper_nomain.o;\
	fi

${OBJECTDIR}/config_nomain.o: ${OBJECTDIR}/config.o config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/config_nomain.o config.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/config.o ${OBJECTDIR}/config_nomain.o;\
	fi

${OBJECTDIR}/daemon_nomain.o: ${OBJECTDIR}/daemon.o daemon.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/daemon.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/daemon_nomain.o daemon.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/daemon.o ${OBJECTDIR}/daemon_nomain.o;\
	fi

${OBJECTDIR}/database_nomain.o: ${OBJECTDIR}/database.o database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/database.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/database_nomain.o database.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/database.o ${OBJECTDIR}/database_nomain.o;\
	fi

${OBJECTDIR}/databases_nomain.o: ${OBJECTDIR}/databases.o databases.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/databases.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/databases_nomain.o databases.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/databases.o ${OBJECTDIR}/databases_nomain.o;\
	fi

${OBJECTDIR}/document_nomain.o: ${OBJECTDIR}/document.o document.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/document.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_nomain.o document.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/document.o ${OBJECTDIR}/document_nomain.o;\
	fi

${OBJECTDIR}/document_attachment_nomain.o: ${OBJECTDIR}/document_attachment.o document_attachment.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/document_attachment.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_attachment_nomain.o document_attachment.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/document_attachment.o ${OBJECTDIR}/document_attachment_nomain.o;\
	fi

${OBJECTDIR}/document_collection_nomain.o: ${OBJECTDIR}/document_collection.o document_collection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/document_collection.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_collection_nomain.o document_collection.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/document_collection.o ${OBJECTDIR}/document_collection_nomain.o;\
	fi

${OBJECTDIR}/document_collection_results_nomain.o: ${OBJECTDIR}/document_collection_results.o document_collection_results.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/document_collection_results.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_collection_results_nomain.o document_collection_results.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/document_collection_results.o ${OBJECTDIR}/document_collection_results_nomain.o;\
	fi

${OBJECTDIR}/document_revision_nomain.o: ${OBJECTDIR}/document_revision.o document_revision.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/document_revision.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_revision_nomain.o document_revision.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/document_revision.o ${OBJECTDIR}/document_revision_nomain.o;\
	fi

${OBJECTDIR}/documents_nomain.o: ${OBJECTDIR}/documents.o documents.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/documents.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/documents_nomain.o documents.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/documents.o ${OBJECTDIR}/documents_nomain.o;\
	fi

${OBJECTDIR}/get_all_documents_options_nomain.o: ${OBJECTDIR}/get_all_documents_options.o get_all_documents_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/get_all_documents_options.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/get_all_documents_options_nomain.o get_all_documents_options.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/get_all_documents_options.o ${OBJECTDIR}/get_all_documents_options_nomain.o;\
	fi

${OBJECTDIR}/get_view_options_nomain.o: ${OBJECTDIR}/get_view_options.o get_view_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/get_view_options.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/get_view_options_nomain.o get_view_options.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/get_view_options.o ${OBJECTDIR}/get_view_options_nomain.o;\
	fi

${OBJECTDIR}/http_server_nomain.o: ${OBJECTDIR}/http_server.o http_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/http_server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server_nomain.o http_server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/http_server.o ${OBJECTDIR}/http_server_nomain.o;\
	fi

${OBJECTDIR}/http_server_log_nomain.o: ${OBJECTDIR}/http_server_log.o http_server_log.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/http_server_log.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server_log_nomain.o http_server_log.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/http_server_log.o ${OBJECTDIR}/http_server_log_nomain.o;\
	fi

${OBJECTDIR}/json_stream_nomain.o: ${OBJECTDIR}/json_stream.o json_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/json_stream.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/json_stream_nomain.o json_stream.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/json_stream.o ${OBJECTDIR}/json_stream_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_nomain.o: ${OBJECTDIR}/map_reduce.o map_reduce.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_nomain.o map_reduce.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce.o ${OBJECTDIR}/map_reduce_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_query_key_nomain.o: ${OBJECTDIR}/map_reduce_query_key.o map_reduce_query_key.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_query_key.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_query_key_nomain.o map_reduce_query_key.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_query_key.o ${OBJECTDIR}/map_reduce_query_key_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_result_nomain.o: ${OBJECTDIR}/map_reduce_result.o map_reduce_result.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_result.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_result_nomain.o map_reduce_result.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_result.o ${OBJECTDIR}/map_reduce_result_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_result_array_nomain.o: ${OBJECTDIR}/map_reduce_result_array.o map_reduce_result_array.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_result_array.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_result_array_nomain.o map_reduce_result_array.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_result_array.o ${OBJECTDIR}/map_reduce_result_array_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_result_comparers_nomain.o: ${OBJECTDIR}/map_reduce_result_comparers.o map_reduce_result_comparers.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_result_comparers.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_result_comparers_nomain.o map_reduce_result_comparers.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_result_comparers.o ${OBJECTDIR}/map_reduce_result_comparers_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_results_nomain.o: ${OBJECTDIR}/map_reduce_results.o map_reduce_results.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_results.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_results_nomain.o map_reduce_results.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_results.o ${OBJECTDIR}/map_reduce_results_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_results_iterator_nomain.o: ${OBJECTDIR}/map_reduce_results_iterator.o map_reduce_results_iterator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_results_iterator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_results_iterator_nomain.o map_reduce_results_iterator.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_results_iterator.o ${OBJECTDIR}/map_reduce_results_iterator_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_shard_results_nomain.o: ${OBJECTDIR}/map_reduce_shard_results.o map_reduce_shard_results.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_shard_results.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_shard_results_nomain.o map_reduce_shard_results.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_shard_results.o ${OBJECTDIR}/map_reduce_shard_results_nomain.o;\
	fi

${OBJECTDIR}/map_reduce_thread_pool_nomain.o: ${OBJECTDIR}/map_reduce_thread_pool.o map_reduce_thread_pool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/map_reduce_thread_pool.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map_reduce_thread_pool_nomain.o map_reduce_thread_pool.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/map_reduce_thread_pool.o ${OBJECTDIR}/map_reduce_thread_pool_nomain.o;\
	fi

${OBJECTDIR}/post_all_documents_options_nomain.o: ${OBJECTDIR}/post_all_documents_options.o post_all_documents_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/post_all_documents_options.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/post_all_documents_options_nomain.o post_all_documents_options.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/post_all_documents_options.o ${OBJECTDIR}/post_all_documents_options_nomain.o;\
	fi

${OBJECTDIR}/rest_config_nomain.o: ${OBJECTDIR}/rest_config.o rest_config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/rest_config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_config_nomain.o rest_config.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/rest_config.o ${OBJECTDIR}/rest_config_nomain.o;\
	fi

${OBJECTDIR}/rest_exceptions_nomain.o: ${OBJECTDIR}/rest_exceptions.o rest_exceptions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/rest_exceptions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_exceptions_nomain.o rest_exceptions.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/rest_exceptions.o ${OBJECTDIR}/rest_exceptions_nomain.o;\
	fi

${OBJECTDIR}/rest_server_nomain.o: ${OBJECTDIR}/rest_server.o rest_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/rest_server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_server_nomain.o rest_server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/rest_server.o ${OBJECTDIR}/rest_server_nomain.o;\
	fi

${OBJECTDIR}/script_array_jsapi_key_value_source_nomain.o: ${OBJECTDIR}/script_array_jsapi_key_value_source.o script_array_jsapi_key_value_source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/script_array_jsapi_key_value_source.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_array_jsapi_key_value_source_nomain.o script_array_jsapi_key_value_source.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/script_array_jsapi_key_value_source.o ${OBJECTDIR}/script_array_jsapi_key_value_source_nomain.o;\
	fi

${OBJECTDIR}/script_array_jsapi_source_nomain.o: ${OBJECTDIR}/script_array_jsapi_source.o script_array_jsapi_source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/script_array_jsapi_source.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_array_jsapi_source_nomain.o script_array_jsapi_source.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/script_array_jsapi_source.o ${OBJECTDIR}/script_array_jsapi_source_nomain.o;\
	fi

${OBJECTDIR}/script_object_jsapi_source_nomain.o: ${OBJECTDIR}/script_object_jsapi_source.o script_object_jsapi_source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/script_object_jsapi_source.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_object_jsapi_source_nomain.o script_object_jsapi_source.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/script_object_jsapi_source.o ${OBJECTDIR}/script_object_jsapi_source_nomain.o;\
	fi

${OBJECTDIR}/script_object_response_stream_nomain.o: ${OBJECTDIR}/script_object_response_stream.o script_object_response_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/script_object_response_stream.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_object_response_stream_nomain.o script_object_response_stream.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/script_object_response_stream.o ${OBJECTDIR}/script_object_response_stream_nomain.o;\
	fi

${OBJECTDIR}/set_thread_name_nomain.o: ${OBJECTDIR}/set_thread_name.o set_thread_name.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/set_thread_name.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/set_thread_name_nomain.o set_thread_name.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/set_thread_name.o ${OBJECTDIR}/set_thread_name_nomain.o;\
	fi

${OBJECTDIR}/uuid_helper_nomain.o: ${OBJECTDIR}/uuid_helper.o uuid_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/uuid_helper.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/src/libscriptobject_msgpack -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src -I../../externals/libjsapi/externals/installed/include/mozjs- -I../../externals/thread-pool-cpp/thread_pool -I../../externals/libscriptobject/externals/msgpack-c/include -I../../externals/ConstTimeEncoding -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/uuid_helper_nomain.o uuid_helper.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/uuid_helper.o ${OBJECTDIR}/uuid_helper_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb

# Subprojects
.clean-subprojects:
	cd ../../externals/libhttpserver/src/libhttpserver && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libscriptobject/src/libscriptobject_gason && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libscriptobject/src/libscriptobject && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libjsapi/src/libjsapi && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libscriptobject/src/libscriptobject_msgpack && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
