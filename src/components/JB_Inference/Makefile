CFLAGS:=${GRAPHVIZ_HEADERS} ${CFLAGS}
LDFLAGS:=${GRAPHVIZ_LIBRARIES} ${LDFLAGS} -lm
FILES=VertexSet.c JB_Inference.c
OFILES=VertexSet.o JB_Inference.o
all: ${FILES}
	gcc -c ${CFLAGS} ${LDFLAGS} ${FILES}
	ar rcs ${BIN}/JB_Inference.a ${OFILES}