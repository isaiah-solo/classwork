#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbox.h"

struct cbox {
    int value;
};

cbox *cbox_new (void) {
    return cbox_new_int(0);
}

cbox *cbox_new_int(int value) {
    cbox *this = malloc(sizeof (struct cbox));
    assert(this != NULL);
    this->value = value;
    return this;
}

void cbox_free(cbox *this) {
    free(this);
}

int cbox_get(cbox *this) {
    return this->value;
}

void cbox_put(cbox *this, int newvalue) {
    this->value = newvalue;
}

// 
// Notes that would normally not be put in the file:
// 
// A '.c' file always includes its own header.
// 
// The 'struct' definition itself is specified in the
// implementation file.  Everything declared in the implementation
// file is 'private'.  Never put field definitions in a header
// file.
// 
