# Isaiah Solomon
# icsolomo
# CMPS 12B
# Makefile
# Makefile for Programming Assignment 4

JAVASRC    = cyoa.java map.java list.java stack.java
SOURCES    = ${JAVASRC} Makefile
CLASSES    = ${JAVASRC:.java=.class}
JARFILE    = cyoa
FILE       = demo.adventure

all : ${CLASSES}

%.class : %.java
	javac -Xlint $<

test : all
	java ${JARFILE} ${FILE}

clean :
	rm -f *.class

spotless : clean
	rm -f ${JARFILE}

.PHONY: all test clean spotless submit
