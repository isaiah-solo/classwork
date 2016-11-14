# Isaiah Solomon
# icsolomo

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wfatal-errors -g
SOURCE = graph01.c intList.c
OBJECTS = ${SOURCE:.c=.o}

graph01: ${OBJECTS}
	${CC} -o graph01 ${CFLAGS} ${OBJECTS}

graph01.o: graph01.c intList.h
	${CC} -c ${CFLAGS} graph01.c

intList.o: intList.c intList.h
	${CC} -c ${CFLAGS} intList.c

clean:
	rm -f *.o graph01

.PHONY: clean