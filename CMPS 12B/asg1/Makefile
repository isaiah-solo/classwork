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
JAVASRC = BusinessSearch.java
SOURCES = README makefile ${JAVASRC}
MAINCLASS = BusinessSearch
CLASSES = BusinessSearch.class Business.class
JARFILE = BusinessSearch
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
