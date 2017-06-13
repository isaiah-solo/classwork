CC = cc
CFLAGS = -std=c99 -Wall -Werror -Wfatal-errors -g
SOURCE = mycat.c
OBJECTS = ${SOURCE:.c=.o}

mycat: ${OBJECTS}
	${CC} -o mycat ${CFLAGS} ${OBJECTS}
	rm ${OBJECTS}

mycat.o: mycat.c
	${CC} -c ${CFLAGS} mycat.c

clean:
	rm -f *.o *.core mycat
