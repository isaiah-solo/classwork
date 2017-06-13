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

#include "rijndael.h"

static char rcsid[] = "$Id: encrypt.c,v 1.2 2003/04/15 01:05:36 elm Exp elm $";

#define KEYBITS 128
#define SETKEY 548

int
encrypt_file (unsigned long k0, unsigned long k1, struct stat file_stats, char* filename)
{
  unsigned long rk[RKLENGTH(KEYBITS)];		/* round key */
  unsigned char key[KEYLENGTH(KEYBITS)];	/* cipher key */
  unsigned char filedata[16];
  unsigned char ciphertext[16];
  unsigned char ctrvalue[16];
  int fd;
  int nrounds;
  int nbytes, nwritten , ctr;
  int totalbytes;
  int fileId = file_stats.st_ino;

  bcopy (&k0, &(key[0]), sizeof (k0));
  bcopy (&k1, &(key[sizeof(k0)]), sizeof (k1));

  /*
   * Initialize the Rijndael algorithm.  The round key is initialized by this
   * call from the values passed in key and KEYBITS.
   */
  nrounds = rijndaelSetupEncrypt(rk, key, KEYBITS);
  
  /*
   * Open the file.
   */
  fd = open(filename, O_RDWR);
  if (fd < 0)
  {
    fprintf(stderr, "Error opening file %s\n", filename);
    return 1;
  }

  /* fileID goes into bytes 8-11 of the ctrvalue */
  bcopy (&fileId, &(ctrvalue[8]), sizeof (fileId));

  /* This loop reads 16 bytes from the file, XORs it with the encrypted
     CTR value, and then writes it back to the file at the same position.
     Note that CTR encryption is nice because the same algorithm does
     encryption and decryption.  In other words, if you run this program
     twice, it will first encrypt and then decrypt the file.
  */
  for (ctr = 0, totalbytes = 0; /* loop forever */; ctr++)
  {
    /* Read 16 bytes (128 bits, the blocksize) from the file */
    nbytes = read (fd, filedata, sizeof (filedata));
    if (nbytes <= 0) {
      break;
    }
    if (lseek (fd, totalbytes, SEEK_SET) < 0)
    {
      perror ("Unable to seek back over buffer");
      exit (-1);
    }

    /* Set up the CTR value to be encrypted */
    bcopy (&ctr, &(ctrvalue[0]), sizeof (ctr));

    /* Call the encryption routine to encrypt the CTR value */
    rijndaelEncrypt(rk, nrounds, ctrvalue, ciphertext);

    /* XOR the result into the file data */
    for (int i = 0; i < nbytes; i++) {
      filedata[i] ^= ciphertext[i];
    }

    /* Write the result back to the file */
    nwritten = write(fd, filedata, nbytes);
    if (nwritten != nbytes)
    {
      fprintf (stderr,
	       "protectfile: error writing the file (expected %d, got %d at ctr %d\n)",
	       nbytes, nwritten, ctr);
      break;
    }

    /* Increment the total bytes written */
    totalbytes += nbytes;
  }
  close (fd);
  return 0;
}

int
main (int argc, char **argv)
{
  int c = getopt (argc, argv, "ed:");
  int encryptflag = 0;
  struct stat file_stats;

  int arg4 = (argc == 4);

  if (!arg4 || c == -1) {
    printf("usage: protectfile <-e | -d> <key> <path>\n");
    return 1;
  }

  unsigned long k = strtol(argv[2], NULL, 0);
  unsigned long k0 = k & 0xffffffff;
  unsigned long k1 = k >> 32;
  char* filename = argv[3];

  stat(filename, &file_stats);
  mode_t mode = file_stats.st_mode;
  encryptflag = (mode & S_ISVTX);

  switch (c) {
    case 'e':
      if (encryptflag != 0) {
        printf("Already encrypted\n");
	return 1;
      }
      break;

    case 'd':
      if (encryptflag == 0) {
	printf("Already decrypted\n");
	return 1;
      }
      break;

    default:
      abort ();
  }

  mode_t new_mode = (mode ^ S_ISVTX);

  syscall(SETKEY, k0, k1);
  encrypt_file(k0, k1, file_stats, filename);
  chmod(filename, new_mode);
}

