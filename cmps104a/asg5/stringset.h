// $Id: stringset.h,v 1.1 2015-10-26 14:13:57-07 - - $

#ifndef __STRINGSET__
#define __STRINGSET__

#include <iostream>
#include <string>

const std::string* intern_stringset (const char*);

void dump_stringset (std::ostream&);

#endif

