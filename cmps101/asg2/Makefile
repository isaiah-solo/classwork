# Isaiah Solomon
# icsolomo

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wfatal-errors -g
MAIN = graph02
SOURCE = ${MAIN}.c intList.c
OBJECTS = ${SOURCE:.c=.o}

${MAIN}: ${OBJECTS}
	${CC} -o ${MAIN} ${CFLAGS} ${OBJECTS}

${MAIN}.o: ${MAIN}.c intList.h
	${CC} -c ${CFLAGS} ${MAIN}.c

intList.o: intList.c intList.h
	${CC} -c ${CFLAGS} intList.c

clean:
	rm -f *.o ${MAIN}

.PHONY: clean
