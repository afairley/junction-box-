# Makefile for JunctionBox 0.01
# Sets up environment for recursive invocations of make in subdirs
export
######################################################################
#### Set up paths to resources
#### These require updating on a new machine
#### TODO: Don't do this... 
######################################################################
# Users should set JBHOME to point at the directory containing this
# Makefile. We do it if it isn't done yet. 
JBHOME ?= ${shell pwd}
OS=linux
######################################################################
####
#### JUNCTION_BOX PRODUCTS
####
######################################################################
INCLUDE:=${JBHOME}/include
BIN:=${JBHOME}/bin
TESTBIN:=${JBHOME}/bin/test
SRCDIR:=${JBHOME}/src
JB_UTIL_LDFLAGS:=-lm
#Products
JB_UTIL_LIB_PATH:=${BIN}/JB_Util.a
JB_UTIL_OBJ=src/components/JB_Util/BitVectorMap.o src/components/JB_Util/BitVector.o src/components/JB_Util/DSetForest.o src/components/JB_Util/IntMap.o src/components/JB_Util/JB_Error.o src/components/JB_Util/JB_Mem.o src/components/JB_Util/JB_String.o src/components/JB_Util/LinkedList.o src/components/JB_Util/Map.o src/components/JB_Util/Variable.o src/components/JB_Util/VariableOrdering.o src/components/JB_Util/VoidpMap.o 
JB_GRAPH_LIB_PATH:=${BIN}/JB_Graph.o 
JB_JT_LIB_PATH:=${BIN}/JB_JunctionTree.o
JB_POT_LIB_PATH:=${BIN}/JB_Pot.o 
JB_MODEL_LIB_PATH:=${BIN}/JB_Model.o
JB_INFERENCE_LIB_PATH:=${BIN}/JB_Inference.a
JB_INFERENCE_OBJ=src/components/JB_Inference/JB_Inference.o src/components/JB_Inference/VertexSet.o
PRODUCTS:=${JB_UTIL_OBJ} ${JB_GRAPH_LIB_PATH} ${JB_JT_LIB_PATH} \
	  ${JB_POT_LIB_PATH} ${JB_MODEL_LIB_PATH} ${JB_INFERENCE_OBJ} 
LIB_NAME=libjunctionbox.so.1
LIB_MINOR_VERSION:=.0.1
LIB_DEST:=${BIN}/${LIB_NAME}${LIB_MINOR_VERSION} 
SWIG_I_DIR:=${SRCDIR}/swig_i
SWIG_I_FILE_JAVA:=${SWIG_I_DIR}/junctionbox_java.i
SWIG_SRC_OUT_DIR:=${SRCDIR}/swig_i/edu/umb/junctionbox
SWIG_SRC_OUT_JAVA_DIR:=${SWIG_SRC_OUT_DIR}/java
SWIG_JAVA_INCLUDES:=-I/home/afairley/local/jdk1.6.0_07/include \
			-I/home/afairley/local/jdk1.6.0_07/include/${OS}
SWIG_WRAPPER:=${SWIG_I_DIR}/junctionbox_java_wrap.c
JAVA_PARSER_DIR=${SRCDIR}/java/
######################################################################
####
#### CHECK Unit Test Framework
####
######################################################################
CHECK_INCLUDE=-I${JBHOME}/lib/bin/check/include
#The path to the check library needs to be provided to 
#test builds. 
#TODO: Move check into build tree, and put the resulting build in
#lib/bin
CHECK_LIB_PATH=${JBHOME}/lib/bin/check/lib/libcheck.a
CHECK_LINK=-lcheck
######################################################################
####
#### GRAPHVIZ
####
######################################################################
# Needs to point at the root of a graphviz install tree produced
# by make install
# Initially using version 2.16
#GRAPHVIZ_VERSION=2.16
#GRAPHVIZ_HOME=${JBHOME}/lib/bin/graphviz
#GRAPHVIZ_LIB=${GRAPHVIZ_HOME}/lib
#GRAPHVIZ_INCLUDE=${GRAPHVIZ_HOME}/include
#GRAPHVIZ_HEADERS=-I${GRAPHVIZ_INCLUDE} \
#			 -I${GRAPHVIZ_INCLUDE}/graphviz
#GRAPHVIZ_LIBRARIES=${GRAPHVIZ_LIB}/libcgraph.so ${GRAPHVIZ_LIB}/libcdt.so

######################################################################
####
#### COMPILER FLAGS
####
######################################################################
CC=gcc
DEBUG=-ggdb3
OPTIMIZE=#-o2
VERBOSE=#-v
WARN=-Wall
FPIC=-fpic
LIBRARY_BUILD_FLAGS=-ggdb3 -shared -Wl,-soname,${LIB_NAME} \
	${SWIG_WRAPPER}		\
	-o ${LIB_DEST} ${PRODUCTS} \
			-lm

CFLAGS:=${WARN}  ${DEBUG} ${FPIC} ${OPTIMIZE} \
	${VERBOSE} -I${INCLUDE}

SUBDIRS:=src
.PHONY: ${SUBDIRS} clean
all:${SUBDIRS}
${SUBDIRS}:
	@echo 'JBHOME is ' ${JBHOME}
	${MAKE} -C $@ all

library:
	${CC} ${SWIG_JAVA_INCLUDES} ${CFLAGS} ${GRAPHVIZ_HEADERS} ${LIBRARY_BUILD_FLAGS} 
#SWIG gleefully ignores -outdir as far as generated C files are concerned
javaswig:
	cd  ${SWIG_SRC_OUT_DIR} && \
	swig -java -package edu.umb.junctionbox -outdir ${SWIG_SRC_OUT_DIR} ${SWIG_I_FILE_JAVA} && \
	gcc  -I${INCLUDE} ${SWIG_JAVA_INCLUDES} -c \
	${SRCDIR}/swig_i/junctionbox_java_wrap.c && \
	cd -
swigtest:
	cp ${LIB_DEST} ${SWIG_SRC_OUT_DIR}/ &&\
	cd  ${SWIG_SRC_OUT_DIR} && \
	rm *.class ; \
	cd ${SRCDIR}/swig_i && \
	ant run && \
	cp junctionbox.jar ${LIB_DEST} ${JAVA_PARSER_DIR} 
parser:
	cd ${JAVA_PARSER_DIR} && \
	ant package

