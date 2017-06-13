/*
 * This code encrypts input data using the Rijndael (AES) cipher.  The
 * key length is hard-coded to 128 key bits; this number may be changed
 * by redefining a constant near the start of the file.
 *
 * This program uses CTR mode encryption.
 *
 * Usage: encrypt <key1> <key2> <file name>
 *
 * Author: Ethan L. Miller (elm@cs.ucsc.edu)
 * Based on code from Philip J. Erdelsky (pje@acm.org)
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/vnode.h>

#define SETKEY 548

int
main (int argc, char **argv)
{
  int arg2 = (argc == 2);

  if (!arg2) {
    printf("usage: setkey <key>\n");
    return 1;
  }

  unsigned long k = strtol(argv[1], NULL, 0);
  unsigned long k0 = k & 0xffffffff;
  unsigned long k1 = k >> 32;

  syscall(SETKEY, k0, k1);
}

