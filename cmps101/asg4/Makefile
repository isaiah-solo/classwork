# Isaiah Solomon
# icsolomo

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wfatal-errors -g
MAIN = graph04
SOURCE = edgeList.c loadWgtGraph.c minPQ.c
OBJECTS = ${SOURCE:.c=.o} ${MAIN}.o
HEADERS = ${SOURCE:.c=.h}

${MAIN}: ${OBJECTS}
	${CC} -o ${MAIN} ${CFLAGS} ${OBJECTS}

graph04.o: graph04.c ${HEADERS}
	${CC} -c ${CFLAGS} ${MAIN}.c

edgeList.o: edgeList.c ${HEADERS}
	${CC} -c ${CFLAGS} edgeList.c

loadWgtGraph.o: loadWgtGraph.c ${HEADERS}
	${CC} -c ${CFLAGS} loadWgtGraph.c

minPQ.o: minPQ.c ${HEADERS}
	${CC} -c ${CFLAGS} minPQ.c

clean:
	rm -f *.o ${MAIN}

.PHONY: clean
