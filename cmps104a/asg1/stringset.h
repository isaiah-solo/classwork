// $Id: stringset.h,v 1.4 2014-10-09 15:42:13-07 - - $

#ifndef __STRINGSET__
#define __STRINGSET__

#include <iostream>
#include <string>

const std::string* intern_stringset (const char*);

void dump_stringset (std::ostream&);

#endif

