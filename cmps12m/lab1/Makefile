# Isaiah Solomon
# icsolomo
# CMPS 12M
# makefile.sh
# This file assigns files to variables, then it cleans or deletes previous
#   versions of hello.class, hello2.class, and myHello. After, it creates
#   a Manifest file and writes a message into the file, then compiles
#   everything together, deletes the Manifest file, then makes myHello an
#   executable.

# A simple makefile with variables
JAVASRC = hello.java hello2.java
SOURCES = README makefile ${JAVASRC}
MAINCLASS = hello2
CLASSES = hello.class hello2.class
JARFILE = myHello
JARCLASSES = ${CLASSES}
all: ${JARFILE}
${JARFILE}: ${CLASSES}
	echo "Main-class: ${MAINCLASS}" > Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	rm Manifest
	chmod +x ${JARFILE}
${CLASSES}: ${JAVASRC}
	javac -Xlint ${JAVASRC}
clean:
	rm ${CLASSES} ${JARFILE}
.PHONY: clean all
