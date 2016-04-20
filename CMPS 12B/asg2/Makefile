# Isaiah Solomon
# icsolomo
# 1344680
# CMPS 12B
# edfile.java
# Template for a line-oriented text editor inspired by ed.
# Makefile for Programming Assignment 2

JAVASRC    = edfile.java dllist.java auxlib.java dllistTest.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = edfile
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES} dllist\$$*.class
JARFILE    = edfile
SUBMITDIR  = cmps12b-nojw.f14 asg2
TESTCLASS  = dllist
CLASSPATH  = "junit-4.12-beta-1.jar:hamcrest-core-1.3.jar:."

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo "Main-class: ${MAINCLASS}" >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	chmod +x ${JARFILE}
	rm Manifest

%.class : %.java
	javac -cp ${CLASSPATH} $<

test : ${TESTCLASS}Test.class ${TESTCLASS}.class
	java -cp ${CLASSPATH} org.junit.runner.JUnitCore ${TESTCLASS}Test

clean :
	rm -f *.class Manifest

spotless : clean
	rm -f ${JARFILE}

submit : ${SOURCES}
	submit ${SUBMITDIR} ${SOURCES}

.PHONY:all test clean spotless submit
