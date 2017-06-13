#
# Makefile for Rijndael (AES) code.
#
# $Id: Makefile,v 1.2 2003/04/15 00:26:33 elm Exp elm $
# NOTE: has GNU make specific stuff

SRCS = setkey.c
SRCOBJS = setkey.o
PROGS = setkey
OTHERS = Makefile
DIR = $(notdir $(PWD))
TAR = tar
TARFILE = aes.tgz

CFLAGS = -O3

all: $(PROGS)

setkey: setkey.o
	$(CC) $(CFLAGS) -o $@ setkey.o
	rm -f *.o

tarball:
	cd .. ; $(TAR) cvf - $(addprefix $(DIR)/, $(SRCS) $(OTHERS)) | gzip - > $(TARFILE)

clean:
	rm -f ${PROGS}

spotless: clean
	$(RM) $(PROGS)
