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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1383664149/city.o \
	${OBJECTDIR}/database.o \
	${OBJECTDIR}/databases.o \
	${OBJECTDIR}/document.o \
	${OBJECTDIR}/document_revision.o \
	${OBJECTDIR}/documents.o \
	${OBJECTDIR}/get_all_documents_options.o \
	${OBJECTDIR}/http_server.o \
	${OBJECTDIR}/http_server_log.o \
	${OBJECTDIR}/json_stream.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/post_all_documents_options.o \
	${OBJECTDIR}/rest_config.o \
	${OBJECTDIR}/rest_exceptions.o \
	${OBJECTDIR}/rest_server.o \
	${OBJECTDIR}/script_object_response_stream.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=--coverage
CXXFLAGS=--coverage

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../../externals/libhttpserver/src/libhttpserver/dist/Debug/GNU-Linux-x86/libhttpserver.a ../../externals/libjsapi/src/libjsapi/dist/Debug/GNU-Linux-x86/libjsapi.a ../../externals/libscriptobject/src/libscriptobject_gason/dist/Debug/GNU-Linux-x86/liblibscriptobject_gason.a ../../externals/libscriptobject/src/libscriptobject/dist/Debug/GNU-Linux-x86/libscriptobject.a -lboost_regex -lboost_filesystem -lboost_program_options -lboost_thread -lboost_date_time -lboost_chrono -lboost_system `pkg-config --libs zlib` -lpthread   

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libhttpserver/src/libhttpserver/dist/Debug/GNU-Linux-x86/libhttpserver.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libjsapi/src/libjsapi/dist/Debug/GNU-Linux-x86/libjsapi.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject_gason/dist/Debug/GNU-Linux-x86/liblibscriptobject_gason.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject/dist/Debug/GNU-Linux-x86/libscriptobject.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb ${OBJECTFILES} ${LDLIBSOPTIONS} --coverage

${OBJECTDIR}/_ext/1383664149/city.o: ../../externals/cityhash/src/city.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1383664149
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1383664149/city.o ../../externals/cityhash/src/city.cc

${OBJECTDIR}/database.o: database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/database.o database.cpp

${OBJECTDIR}/databases.o: databases.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/databases.o databases.cpp

${OBJECTDIR}/document.o: document.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document.o document.cpp

${OBJECTDIR}/document_revision.o: document_revision.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/document_revision.o document_revision.cpp

${OBJECTDIR}/documents.o: documents.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/documents.o documents.cpp

${OBJECTDIR}/get_all_documents_options.o: get_all_documents_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/get_all_documents_options.o get_all_documents_options.cpp

${OBJECTDIR}/http_server.o: http_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server.o http_server.cpp

${OBJECTDIR}/http_server_log.o: http_server_log.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server_log.o http_server_log.cpp

${OBJECTDIR}/json_stream.o: json_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/json_stream.o json_stream.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/post_all_documents_options.o: post_all_documents_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/post_all_documents_options.o post_all_documents_options.cpp

${OBJECTDIR}/rest_config.o: rest_config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_config.o rest_config.cpp

${OBJECTDIR}/rest_exceptions.o: rest_exceptions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_exceptions.o rest_exceptions.cpp

${OBJECTDIR}/rest_server.o: rest_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_server.o rest_server.cpp

${OBJECTDIR}/script_object_response_stream.o: script_object_response_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi -I../../externals/termcolor/include -I../../externals/libscriptobject/src/libscriptobject -I../../externals/libscriptobject/src/libscriptobject_gason -I../../externals/libscriptobject/externals/gason/src -I../../externals/cityhash/src `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_object_response_stream.o script_object_response_stream.cpp

# Subprojects
.build-subprojects:
	cd ../../externals/libhttpserver/src/libhttpserver && ${MAKE}  -f Makefile CONF=Debug
	cd ../../externals/libjsapi/src/libjsapi && ${MAKE}  -f Makefile CONF=Debug
	cd ../../externals/libscriptobject/src/libscriptobject_gason && ${MAKE}  -f Makefile CONF=Debug
	cd ../../externals/libscriptobject/src/libscriptobject && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb

# Subprojects
.clean-subprojects:
	cd ../../externals/libhttpserver/src/libhttpserver && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../externals/libjsapi/src/libjsapi && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../externals/libscriptobject/src/libscriptobject_gason && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../externals/libscriptobject/src/libscriptobject && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
