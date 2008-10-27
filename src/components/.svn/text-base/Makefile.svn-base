SUBDIRS:=JB_Util JB_Graph JB_Pot JB_Model JB_JunctionTree JB_Inference 
.PHONY: ${SUBDIRS} clean
all:${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ all
