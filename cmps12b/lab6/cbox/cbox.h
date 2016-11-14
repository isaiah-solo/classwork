#ifndef __CBOX_H__
#define __CBOX_H__

#include <stdbool.h>

// NAME
//    cbox ADT
//
// DESCRIPTION
//    A simple ADT that permits the holding of an integer in a box
//    similar to the way Java uses an `Integer' to box an `int'.

typedef struct cbox cbox;
    // Incomplete type defined in implementation file.

cbox *cbox_new(void);
    // Constructor: create a new `cbox' box initialized to 0.
    // Postcond:    new cbox box is returned.

cbox *cbox_new_int(int value);
    // Constructor: create a new `cbox' box initialized by caller.
    // Postcond:    new cbox box is returned.

void cbox_free(cbox *this);
    // Destructor: destroys an allocated box
    // Precond:    box created by cbox_new.
    // Postcond:   this pointer is dangling.

int cbox_get(cbox *this);
    // Accessor:   retrieves the integer from the box.
    // Precond:    valid handle to an cbox.
    // Postcond:   returns the value in the box.

void cbox_put(cbox *this, int newvalue);
    // Mutator:    replaces the integer in the box with a new one.
    // Precond:    valid handle to an cbox.
    // Postcond:   old value is lost, new value is kept

#endif

// 
// Notes:
// 
// File guards protect the file from multiple inclusion.
// 
// A header file specifies only the prototypes for functions,
// similar to the way an interface does in Java.  Everything in the
// header file is `public'.
// 
// Note that all function names are global and can not be
// overloaded.  So we name a function as in Java and prefix it with
// the name of the `module' that it belongs to.  Note that in
// the standard C library, there are often common prefixes, such as
// `f-' for file-oriented functions, `str-' for string functions, etc.
// 
