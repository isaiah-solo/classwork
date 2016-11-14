# Isaiah Solomon
# icsolomo

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wfatal-errors -g
SOURCE = scc03.c intList.c dfsTrace1.c dfsPhase2.c loadGraph.c
OBJECTS = ${SOURCE:.c=.o}
HEADERS = intList.h dfsTrace1.h dfsPhase2.h loadGraph.h

scc03: ${OBJECTS}
	${CC} -o scc03 ${CFLAGS} ${OBJECTS}

scc03.o: scc03.c ${HEADERS}
	${CC} -c ${CFLAGS} scc03.c

intList.o: intList.c ${HEADERS}
	${CC} -c ${CFLAGS} intList.c

dfsTrace1.o: dfsTrace1.c ${HEADERS}
	${CC} -c ${CFLAGS} dfsTrace1.c

dfsPhase2.o: dfsPhase2.c ${HEADERS}
	${CC} -c ${CFLAGS} dfsPhase2.c

loadGraph.o: loadGraph.c ${HEADERS}
	${CC} -c ${CFLAGS} loadGraph.c

clean:
	rm -f *.o scc03

.PHONY: clean