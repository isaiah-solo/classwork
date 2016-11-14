#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

enum { ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
       ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
       ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param,
       ATTR_lval, ATTR_const, ATTR_vreg, ATTR_vaddr,
       ATTR_bitset_size,
};

struct symbol;

using attr_bitset = bitset<ATTR_bitset_size>;
using symbol_table = unordered_map<string*,symbol*>;
using symbol_entry = symbol_table::value_type;

vector<symbol_table*> symbol_stack;
int next_block = 1;

int init_stack ();
int enter_block ();
int leave_block ();
int define_ident ();
int search_ident ();
