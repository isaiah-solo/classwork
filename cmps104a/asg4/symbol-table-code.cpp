// $Id: symbol-table-code.cpp,v 1.6 2015-05-13 14:40:56-07 - - $

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

using attr_bitset = bitset<ATTR_bitset_size>;
using symbol_table = unordered_map<string*,symbol*>;
using symbol_entry = symbol_table::value_type;

struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr, linenr, offset;
   size_t blocknr;
   vector<symbol*>* parameters;
};

int init_stack () {
   symbol_table = new vector<symbol_table*>();
   next_block = 1;
   return 0;
}

int enter_block () {
   ++next_block;
   symbol_stack.push_back(nullptr);
   return 0;
}

int leave_block () {
   auto return_value = symbol_stack.pop_back();
   return 0;
}

int define_ident () {
   symbol* new_symbol = new symbol();
   new_symbol->fields = new unordered_map<string*, symbol*>();
   symbol_stack.push_back(new_symbol);
   return 0;
}

int search_ident () {

}

void traverse_ast (astree* child) {
   for (auto itor = yyparse_astree->begin();
        itor != yyparse_astree->end(); ++itor) {
      traverse
   }
}
