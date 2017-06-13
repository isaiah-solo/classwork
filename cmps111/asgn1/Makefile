CC = cc
CFLAGS = -std=c99 -g
SOURCE = myshell.c
LSOURCE = lex.l
LGEN = lex.yy.c
OBJECTS = ${SOURCE:.c=.o}
LOBJECTS = ${LGEN:.c=.o}
EXEC = myshell

${EXEC}: ${OBJECTS}
	flex ${LSOURCE}
	${CC} -o ${EXEC} ${OBJECTS} ${LGEN} -lfl
	rm ${OBJECTS}

myshell.o: ${SOURCE}
	${CC} -c ${CFLAGS} ${SOURCE}

clean:
	rm -f ${OBJECTS} ${EXEC} ${LGEN} *.core
