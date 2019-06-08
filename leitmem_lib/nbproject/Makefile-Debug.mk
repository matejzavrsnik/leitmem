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
CND_PLATFORM=GNU-Linux
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
	${OBJECTDIR}/flipcards_from_xml_file.o \
	${OBJECTDIR}/leitmem.o \
	${OBJECTDIR}/ui_cli.o \
	${OBJECTDIR}/utils.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall -Wextra -Wpedantic -std=c++2a
CXXFLAGS=-Wall -Wextra -Wpedantic -std=c++2a

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libleitmem_lib.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libleitmem_lib.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libleitmem_lib.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/flipcards_from_xml_file.o: flipcards_from_xml_file.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I../3rdparty/mzlib/include -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/flipcards_from_xml_file.o flipcards_from_xml_file.cpp

${OBJECTDIR}/leitmem.o: leitmem.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I../3rdparty/mzlib/include -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem.o leitmem.cpp

${OBJECTDIR}/ui_cli.o: ui_cli.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I../3rdparty/mzlib/include -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_cli.o ui_cli.cpp

${OBJECTDIR}/utils.o: utils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I../3rdparty/mzlib/include -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils.o utils.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
