CFLAGS:= ${CHECK_INCLUDE} ${CFLAGS}
LDFLAGS:=${LDFLAGS}
FILES=Test_JB_Util.c
STATIC_LIBS = ${JB_UTIL_LIB_PATH} ${CHECK_LIB_PATH} 

all: Test_JB_Util
Test_JB_Util: Test_JB_Util.c
	gcc  ${CFLAGS} ${LDFLAGS} Test_JB_Util.c \
	${STATIC_LIBS} -o ${TESTBIN}/Test_JB_Util
