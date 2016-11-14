# Isaiah Solomon
# icsolomo
# CMPS 12M
# Makefile.sh
# This file compiles files and makes an executable.

JAVASRC = CalcTest.java
SOURCES = ${JAVASRC} Calc.java junit-4.12-beta-2.jar hamcrest-core-1.3.jar
MAINCLASS = CalcTest.class
CLASSES = ${MAINCLASS} Calc.class
test:
	javac -cp "junit-4.12-beta-2.jar:hamcrest-core-1.3.jar:." CalcTest.java
	java -cp "junit-4.12-beta-2.jar:hamcrest-core-1.3.jar:." org.junit.runner.JUnitCore CalcTest
clean:
	rm ${CLASSES}
.PHONY: clean test
