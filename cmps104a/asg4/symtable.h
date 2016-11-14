//This program was completed using pair programming.
//Partner:  Karol Josef Bustamante (kfbustam@ucsc.edu)
//Partner:  Isaiah Solomon (icsolomo@ucsc.edu)

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <vector>
#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include "lyutils.h"
#include "auxlib.h"
#include "stringset.h"


struct symbol;

enum { ATTR_void, ATTR_bool, ATTR_char, ATTR_int, 
  ATTR_null, ATTR_string, ATTR_struct, ATTR_array,
  ATTR_function, ATTR_variable, ATTR_field, ATTR_typeid, 
  ATTR_param, ATTR_lval, ATTR_const, ATTR_vreg, ATTR_vaddr, 
  ATTR_bitset_size,
};

using symbol_table = unordered_map<const string*, symbol*>;
using attr_bitset = bitset<ATTR_bitset_size>;
// The actual definition of the symbol struct
struct symbol {
  //attibutes will have attr_int, attr_string, etc.
  attr_bitset attributes;
  //all the fields inside ( other than functions)
  symbol_table* fields;
  //all the parameters (for functions)
  vector<symbol*>* parameters;
  size_t blocknum;
  size_t filenr;            // index into filename stack
  size_t linenr;            // line number from source code
  size_t offset;            // offset of token with current line
};

// Create a shorthand notation for a symbol_table

using symbol_entry = symbol_table::value_type;

// Dump an astree to a FILE.
void dump_symboltable (FILE* outfile, astree* root);

string type_check (astree* node, symbol_table table);

#endif
