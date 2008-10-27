CFLAGS:=${CFLAGS}
FILES=JB_Error.c JB_Mem.c JB_String.c BitVector.c  LinkedList.c Map.c IntMap.c VoidpMap.c BitVectorMap.c DSetForest.c Variable.c VariableOrdering.c
OFILES=JB_Error.o JB_Mem.o JB_String.o BitVector.o  LinkedList.o Map.o IntMap.o VoidpMap.o BitVectorMap.o DSetForest.o Variable.o VariableOrdering.o
all: ${FILES} 
	gcc -c ${CFLAGS} ${FILES};
	ar rcs ${BIN}/JB_Util.a ${OFILES}
