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
	${OBJECTDIR}/database.o \
	${OBJECTDIR}/databases.o \
	${OBJECTDIR}/http_server.o \
	${OBJECTDIR}/json_stream.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/rest_exceptions.o \
	${OBJECTDIR}/rest_server.o


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
LDLIBSOPTIONS=../../externals/libhttpserver/src/libhttpserver/dist/Release/GNU-Linux-x86/libhttpserver.a ../../externals/libjsapi/src/libjsapi/dist/Release/GNU-Linux-x86/libjsapi.a ../../externals/libscriptobject/src/libscriptobject/dist/Release/GNU-Linux-x86/libscriptobject.a ../../externals/libscriptobject/src/libscriptobject_gason/dist/Release/GNU-Linux-x86/liblibscriptobject_gason.a -lboost_regex -lboost_filesystem -lboost_program_options -lboost_thread -lboost_date_time -lboost_chrono -lboost_system `pkg-config --libs zlib` -lpthread   

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libhttpserver/src/libhttpserver/dist/Release/GNU-Linux-x86/libhttpserver.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libjsapi/src/libjsapi/dist/Release/GNU-Linux-x86/libjsapi.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject/dist/Release/GNU-Linux-x86/libscriptobject.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ../../externals/libscriptobject/src/libscriptobject_gason/dist/Release/GNU-Linux-x86/liblibscriptobject_gason.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/database.o: database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/database.o database.cpp

${OBJECTDIR}/databases.o: databases.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/databases.o databases.cpp

${OBJECTDIR}/http_server.o: http_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/http_server.o http_server.cpp

${OBJECTDIR}/json_stream.o: json_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/json_stream.o json_stream.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/rest_exceptions.o: rest_exceptions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_exceptions.o rest_exceptions.cpp

${OBJECTDIR}/rest_server.o: rest_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../externals/libhttpserver/src/libhttpserver -I../../externals/libjsapi/src/libjsapi `pkg-config --cflags zlib` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rest_server.o rest_server.cpp

# Subprojects
.build-subprojects:
	cd ../../externals/libhttpserver/src/libhttpserver && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libjsapi/src/libjsapi && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libscriptobject/src/libscriptobject && ${MAKE}  -f Makefile CONF=Release
	cd ../../externals/libscriptobject/src/libscriptobject_gason && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avancedb

# Subprojects
.clean-subprojects:
	cd ../../externals/libhttpserver/src/libhttpserver && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libjsapi/src/libjsapi && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libscriptobject/src/libscriptobject && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../externals/libscriptobject/src/libscriptobject_gason && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
