#
# Isaiah Solomon
# icsolomo
# CMPS 12B
# Makefile
# This file is a makefile that compiles, runs, and cleans
#

MKFILE    = Makefile

GCC      = gcc -g -O0 -Wall -Wextra -std=gnu99

CSOURCE   = heap.c
OBJECTS   = ${CSOURCE:.c=.o}
EXECBIN   = heap
SOURCES   = ${CSOURCE} ${MKFILE}

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	${GCC} -c $<

clean :
	rm -f ${OBJECTS} core

spotless : clean
	rm -f ${EXECBIN}

test : ${EXECBIN}
	./${EXECBIN} 10 test.txt

.PHONY: all clean spotless test
