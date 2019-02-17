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
	${OBJECTDIR}/leitmem_answer_recognition.o \
	${OBJECTDIR}/leitmem_flipcards_interaction.o \
	${OBJECTDIR}/leitmem_logic.o \
	${OBJECTDIR}/leitmem_progression.o \
	${OBJECTDIR}/leitmem_questions_left.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall -Wextra -Wpedantic -std=c++17
CXXFLAGS=-Wall -Wextra -Wpedantic -std=c++17

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'../leitmem_lib/dist/Debug/GNU-Linux' -L../leitmem_lib/dist/Debug/GNU-Linux -lleitmem_lib -lxml++-2.6 -lgmock -lgtest

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests
	${CP} ../leitmem_lib/dist/Debug/GNU-Linux/libleitmem_lib.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests: ../leitmem_lib/dist/Debug/GNU-Linux/libleitmem_lib.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/leitmem_answer_recognition.o: leitmem_answer_recognition.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem_answer_recognition.o leitmem_answer_recognition.cpp

${OBJECTDIR}/leitmem_flipcards_interaction.o: leitmem_flipcards_interaction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem_flipcards_interaction.o leitmem_flipcards_interaction.cpp

${OBJECTDIR}/leitmem_logic.o: leitmem_logic.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem_logic.o leitmem_logic.cpp

${OBJECTDIR}/leitmem_progression.o: leitmem_progression.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem_progression.o leitmem_progression.cpp

${OBJECTDIR}/leitmem_questions_left.o: leitmem_questions_left.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem_questions_left.o leitmem_questions_left.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../leitmem_lib && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libleitmem_lib.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests

# Subprojects
.clean-subprojects:
	cd ../leitmem_lib && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
