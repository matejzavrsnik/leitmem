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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/leitmem/all_questions.o \
	${OBJECTDIR}/leitmem/answer_recognition.o \
	${OBJECTDIR}/leitmem/logic.o \
	${OBJECTDIR}/leitmem/on_correct_answer.o \
	${OBJECTDIR}/leitmem/on_incorrect_answer.o \
	${OBJECTDIR}/leitmem/progression.o \
	${OBJECTDIR}/leitmem/questions_left.o \
	${OBJECTDIR}/leitmem/store_edges.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../leitmem_lib/dist/Release/GNU-Linux' -L../leitmem_lib/dist/Release/GNU-Linux -lleitmem_lib -lxml++-2.6 -lgmock -lgtest

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests: ../leitmem_lib/dist/Release/GNU-Linux/libleitmem_lib.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/leitmem/all_questions.o: leitmem/all_questions.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/all_questions.o leitmem/all_questions.cpp

${OBJECTDIR}/leitmem/answer_recognition.o: leitmem/answer_recognition.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/answer_recognition.o leitmem/answer_recognition.cpp

${OBJECTDIR}/leitmem/logic.o: leitmem/logic.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/logic.o leitmem/logic.cpp

${OBJECTDIR}/leitmem/on_correct_answer.o: leitmem/on_correct_answer.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/on_correct_answer.o leitmem/on_correct_answer.cpp

${OBJECTDIR}/leitmem/on_incorrect_answer.o: leitmem/on_incorrect_answer.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/on_incorrect_answer.o leitmem/on_incorrect_answer.cpp

${OBJECTDIR}/leitmem/progression.o: leitmem/progression.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/progression.o leitmem/progression.cpp

${OBJECTDIR}/leitmem/questions_left.o: leitmem/questions_left.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/questions_left.o leitmem/questions_left.cpp

${OBJECTDIR}/leitmem/store_edges.o: leitmem/store_edges.cpp
	${MKDIR} -p ${OBJECTDIR}/leitmem
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/leitmem/store_edges.o leitmem/store_edges.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../leitmem_lib -I../../mzlib/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../leitmem_lib && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libleitmem_lib.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/leitmem_tests

# Subprojects
.clean-subprojects:
	cd ../leitmem_lib && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
