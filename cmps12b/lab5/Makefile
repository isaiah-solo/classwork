# Isaiah Solomon
# icsolomo
# 1344680
# CMPS 12B
# Makefile
# Compiles, cleans, tests, and submits

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wfatal-errors -g
OBJS = list.o
OUTPUT = list

${OUTPUT}: ${OBJS}
	${CC} $< -o ${OUTPUT}

%.o: %.c
	${CC} -c ${CFLAGS} $<

clean:
	rm -f *.o

spotless: clean
	rm -f ${OUTPUT}

test: ${OUTPUT}
	valgrind ./${OUTPUT}

.PHONY: clean spotless test

