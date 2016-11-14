//
// Silly main program which just creates an cbox box, puts a
// number in it, gets it back out, and deletes the box.
// Run with bcheck to verify no memory leaks.
//

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbox.h"

int main(int argc, char **argv) {
    // Silence unused warnings
    argc = argc;
    argv = argv;

    // Declare the box and initialize it.
    cbox *box = cbox_new();
    printf("box = %p\n", box);

    // Perform a couple of operations on it.
    cbox_put(box, 1024);
    printf("box value is %d\n", cbox_get(box));

    // Free up the box.
    cbox_free(box);

    return EXIT_SUCCESS;
}

