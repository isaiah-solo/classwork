#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"


//This program was completed using pair programming.
//Partner:  Karol Josef Bustamante (kfbustam@ucsc.edu)
//Partner:  Isaiah Solomon (icsolomo@ucsc.edu)

string tabLinesSym = "";
int next_block = 0;
vector<symbol_table*> symbol_stack;


/** Search symbol table for ident */
static symbol* search_table (string tname, symbol_table* table) {
  auto sym = table->find(&tname);

  // check fields if struct
  if (sym == table->end()) {
    // goes through each symbol in symbol table
    for (auto type_id = table->begin(); type_id != table->end(); ++type_id) {
      auto field = type_id->second->fields->find(&tname);

      // if found
      if (field != type_id->second->fields->end())
        return field->second;
    }
    return nullptr;
  }

  return sym->second;

  // TODO: need to check if struct
}


/** Search symbol stack for ident */
static symbol* search_stack (string tname) {
  // search each symbol table in the stack
  for (auto table = symbol_stack.rbegin(); table != symbol_stack.rend(); ++table) {
    // check if table is null
    if (*table == nullptr) continue;

    // search symbol table
    auto sym = search_table(tname, *table);

    // if symbol not found
    if (sym == nullptr) continue;

    return sym;
  }
  return nullptr;
}

static symbol_table* get_struct_fields (symbol_table* table,
                                        astree* node) {
  // create new symbol
  symbol* new_symbol = new symbol();
  new_symbol->attributes.set(ATTR_field);
  new_symbol->fields = nullptr;
  new_symbol->parameters = nullptr;
  new_symbol->filenr = node->children.at(0)->filenr;
  new_symbol->linenr = node->children.at(0)->linenr;
  new_symbol->offset = node->children.at(0)->offset;

  switch (node->symbol) {
    case TOK_INT: new_symbol->attributes.set(ATTR_int); break;
    case TOK_CHAR: new_symbol->attributes.set(ATTR_char); break;
    case TOK_BOOL: new_symbol->attributes.set(ATTR_bool); break;
    case TOK_STRING: new_symbol->attributes.set(ATTR_string); break;
    case TOK_TYPEID: new_symbol->attributes.set(ATTR_typeid); break;
    default: return table;
  }

  // get field name
  const string* field = node->children.at(0)->lexinfo;

  // insert into symbol table
  symbol_entry entry(field, new_symbol);
  table->insert(entry);

  // recursively add more fields
  if (node->children.size() > 1)
    table = get_struct_fields(table, node->children.at(1));

  // return field table
  return table;
}

static attr_bitset* dump_symbol (astree* node) {

  // Token name
  auto tname = get_yytname (node->symbol);
  
  // Print current node info
  printf("the symbol is: %s\n the lexinfo is: %s\n", 
         tname,node->lexinfo->c_str());
  
  // Put node info into new symbol
  symbol* newSymbol = new symbol();
  newSymbol->filenr = node->filenr;
  newSymbol->linenr = node->linenr;
  newSymbol->offset = node->offset;

  /** Switch */
  switch (node->symbol) {

    /** Root */
    case TOK_ROOT: {
        // set blocknum to 0
        newSymbol->blocknum = 0;

        // create initial global symbol table and push to stack
        symbol_table* table = new symbol_table();
        symbol_stack.push_back(table);
      }
      break;


    /** Operations */
    case '+': case '-': case '*': case'/': case '%':
              case '!': case TOK_ORD: case TOK_CHR: {
        // check if binop
        if (node->children.size() == 2) {
          // get left side and right side
          attr_bitset* left = dump_symbol(node->children.at(0));
          attr_bitset* right = dump_symbol(node->children.at(1));

          // error check
          if (left == nullptr || right == nullptr) return nullptr;

          // type check
          if (! left->test(ATTR_int) || ! left->test(ATTR_const) ||
              ! right->test(ATTR_int) || ! right->test(ATTR_const)) {
            return nullptr;
          }

          // return type
          attr_bitset* attr = new attr_bitset(ATTR_int + ATTR_const);
          return attr;
        }

        // check if unop
        else if (node->children.size() == 1) {
          // get child
          attr_bitset* child = dump_symbol(node->children.at(0));

          // error check
          if (child == nullptr) return nullptr;

          // check which unop operation
          switch (node->symbol) {
            case '+': case '-':
              if (! child->test(ATTR_int) || ! child->test(ATTR_const)) {
                return nullptr;
              }
              break;
            case '!':
              if (! child->test(ATTR_bool) || ! child->test(ATTR_const)) {
                return nullptr;
              }
              break;
            case TOK_ORD:
              if (! child->test(ATTR_char) || ! child->test(ATTR_const)) {
                return nullptr;
              }
              break;
            case TOK_CHR:
              if (! child->test(ATTR_int) || ! child->test(ATTR_const)) {
                return nullptr;
              }
              break;
            default: return nullptr;
          }

          // return type
          attr_bitset* attr = new attr_bitset(ATTR_int + ATTR_const);
          return attr;
        }
      }
      break;


    /** Comparators */
    case TOK_EQ: case TOK_NE: {
        // get left and right
        attr_bitset* left = dump_symbol(node->children.at(0));
        attr_bitset* right = dump_symbol(node->children.at(1));

        // error check
        if (left == nullptr || right == nullptr) return nullptr;

        // type check
        if (! (left->to_ullong() == right->to_ullong())) {
          return nullptr;
        } 
      }
      break;

    case TOK_GT: case TOK_GE: case TOK_LT: case TOK_LE: {
        // get left and right
        attr_bitset* left = dump_symbol(node->children.at(0));
        attr_bitset* right = dump_symbol(node->children.at(1));

        // error check
        if (left == nullptr || right == nullptr) return nullptr;

        // type check
        if ((! left->test(ATTR_int) && ! left->test(ATTR_char) && 
             ! left->test(ATTR_bool)) ||
            (! right->test(ATTR_int) && ! right->test(ATTR_char) &&
             ! right->test(ATTR_bool)) ||
             ! (left->to_ullong() == right->to_ullong())) {
          return nullptr;
        }
      }
      break;


    /** Void Type */
    case TOK_VOID: {
        if (node->children.size() > 1) return nullptr;

      }
      break;


    /** Primitive Types */
    case TOK_BOOL: {
        // get table at back of stack
        auto table = symbol_stack.back();
        if (table == nullptr) table = new symbol_table();

        // if array
        if (node->children.size() == 2) {
          attr_bitset* attr = dump_symbol(node->children.at(1));
          attr->set(ATTR_array);
          
          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_bool) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        // if not array
        else if (node->children.size() == 1) {
          attr_bitset* attr = dump_symbol(node->children.at(0));

          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_bool) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        return attr;
      }
      break;

    case TOK_CHAR: {
        // get table at back of stack
        auto table = symbol_stack.back();
        if (table == nullptr) table = new symbol_table();

        // if array
        if (node->children.size() == 2) {
          attr_bitset* attr = dump_symbol(node->children.at(1));
          attr->set(ATTR_array);
          
          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_char) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        // if not array
        else if (node->children.size() == 1) {
          attr_bitset* attr = dump_symbol(node->children.at(0));

          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_char) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        return attr;
      }
      break;

    case TOK_INT: {
        // get table at back of stack
        auto table = symbol_stack.back();
        if (table == nullptr) table = new symbol_table();

        // if array
        if (node->children.size() == 2) {
          attr_bitset* attr = dump_symbol(node->children.at(1));
          attr->set(ATTR_array);
          
          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_int) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        // if not array
        else if (node->children.size() == 1) {
          attr_bitset* attr = dump_symbol(node->children.at(0));

          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_int) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        // if error
        return nullptr;
      }
      break;


    /** Reference Types */
    case TOK_NULL: {
        // type check
        if (node->children.size() > 0) return nullptr;

        // create attr bit set
        attr_bitset* attr = new attr_bitset();
        attr->set(ATTR_null + ATTR_const);

        return attr;
      }
      break;

    case TOK_STRING: {
        // get table at back of stack
        auto table = symbol_stack.back();
        if (table == nullptr) table = new symbol_table();

        
        // if array
        if (node->children.size() == 2) {
          attr_bitset* attr = dump_symbol(node->children.at(1));
          attr->set(ATTR_array);
          
          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_string) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        // if not array
        else if (node->children.size() == 1) {
          attr_bitset* attr = dump_symbol(node->children.at(0));

          // error check
          if (attr == nullptr) return nullptr;

          // type check
          if ((! attr->test(ATTR_string) || ! attr->test(ATTR_const)) &&
               ! attr->test(ATTR_field)) return nullptr;

          return attr;
        }

        return attr;
      }
      break;


    /** Constants */
    case TOK_INTCON: {
        // type check
        

        // create attr bit set
        attr_bitset* attr = new attr_bitset();
        attr->set(ATTR_int);
        attr->set(ATTR_const);

        return attr;
      }
      break;

    case TOK_CHARCON: {
        // type check
        

        // create attr bit set
        attr_bitset* attr = new attr_bitset();
        attr->set(ATTR_char);
        attr->set(ATTR_const);

        return attr;
      }
      break;

    case TOK_STRINGCON: {
        // type check
        

        // create attr bit set
        attr_bitset* attr = new attr_bitset();
        attr->set(ATTR_string);
        attr->set(ATTR_const);

        return attr;
      }
      break;

    case TOK_TRUE: case TOK_FALSE: {
        // type check
        

        // create attr bit set
        attr_bitset* attr = new attr_bitset();
        attr->set(ATTR_bool);
        attr->set(ATTR_const);

        return attr;
      }
      break;

    /** Struct */
    case TOK_STRUCT: {
        // error check
        if (node->children.size() < 1) return nullptr;

        // get table at back of stack
        auto table = symbol_stack.back();
        if (table == nullptr) table = new symbol_table();

        // get ident
        attr_bitset* attr = dump_symbol(node->children.at(0));

        // error check
        if (attr == nullptr) return nullptr;

        // get ident name
        const string* type_id = new string(get_yytname(
                                    node->children.at(0)->symbol));

        // if in stack already
        if (search_stack(*type_id) != nullptr) return nullptr;

        // create new symbol
        symbol* new_symbol = new symbol();
        new_symbol->attributes.set(ATTR_struct);
        new_symbol->fields = new symbol_table();
        new_symbol->parameters = nullptr;
        new_symbol->filenr = node->filenr;
        new_symbol->linenr = node->linenr;
        new_symbol->offset = node->offset;

        // insert fields
        if (node->children.size() > 1) {
          new_symbol->fields = get_struct_fields(
              new_symbol->fields, node->children.at(1));
        }

        // insert into symbol table
        symbol_entry entry(type_id, new_symbol);
        table->insert(entry);

        return attr;
      }
      break;

    case TOK_FIELD: {

        // search for ident
        symbol* sym = search_stack(tname);

        // if ident not found
        if (sym == nullptr) return nullptr;
        
        // create attr bit set
        attr_bitset* attr = new attr_bitset();
        attr->set(ATTR_bool);
        attr->set(ATTR_const);
      }
      break;




    /**  */
    case TOK_ARRAY: {
        newSymbol->blocknum = next_block;
        newSymbol->attributes[ATTR_array]=true;
        newSymbol->fields = nullptr;
        newSymbol->parameters = nullptr;

        auto newSymTable =  symbol_stack.back();
        if(newSymTable == nullptr)
          newSymTable = new symbol_table();
        symbol_entry entry(node->lexinfo, newSymbol);
        newSymTable->insert(entry);
        symbol_stack.push_back(newSymTable); 
    
        //type check:
        //base[]: contains a collection of other elements, 
        //all of which are of the same type, which may be either 
        //primitive or reference types. Its base type may not be an array type. 
        //This is the only polymorphic type.
      }
      break;

    case TOK_RETURN: {
      }
      break;

    case TOK_CALL: {
        newSymbol->attributes[ATTR_vreg] = true;
        newSymbol->fields = nullptr;
        newSymbol->parameters = nullptr;

        auto newSymTable =  symbol_stack.back();
        if(newSymTable == nullptr)
          newSymTable = new symbol_table();
        symbol_entry entry(node->lexinfo, newSymbol);
        newSymTable->insert(entry);
        symbol_stack.push_back(newSymTable); 
      }
      break;

    case TOK_VARDECL: {
        // get table at back of stack
        auto table = symbol_stack.back();

        // get left side and right side
        attr_bitset* left = dump_symbol(node->children.at(0));
        attr_bitset* right = dump_symbol(node->children.at(1));

        // error check
        if (left == nullptr || right == nullptr) break;

        // create new symbol
        symbol* new_symbol = new symbol();
        new_symbol->attributes.set(left->to_ullong());
        new_symbol->fields = nullptr;
        new_symbol->parameters = nullptr;
        new_symbol->filenr = node->filenr;
        new_symbol->linenr = node->linenr;
        new_symbol->offset = node->offset;

        // insert into symbol table
        symbol_entry entry(node->lexinfo, new_symbol);
        table->insert(entry);

        // TODO: type check
      }
      break;

    case TOK_IDENT: {
        attr_bitset* attr = new attr_bitset(ATTR_field + ATTR_lval);

        // search for ident
        symbol* sym = search_stack(tname);

        // if ident not found
        if (sym == nullptr) break;

        return attr;

        // TODO: check if stack is empty
        //     | error message
      }
      break;

    case TOK_DECLID: {
        attr_bitset* attr = new attr_bitset(ATTR_function);

        // search for ident
        symbol* sym = search_stack(tname);

        // if ident not found
        if (sym == nullptr) return attr;

        return nullptr;

        // TODO: check if stack is empty
        //     | error message
      }
      break;

    case TOK_TYPEID: {
        newSymbol->attributes[ATTR_typeid]=true;
        newSymbol->attributes[ATTR_struct] = true;
        newSymbol->blocknum = 0;
        symbol* symbolFound = nullptr;

        symbolFound = traverse_block(*symbol_stack.back(), tname);

        /*
        for (size_t child = 0; child < node->children.size();
            ++child) {

            symbol* newChildSymbol = new symbol();
            symbol_entry entry(node->lexinfo, newChildSymbol);        
            newSymbol->fields = new symbol_table();
            newSymbol->fields->insert(entry);
        } 
        */

        newSymbol->parameters = nullptr;

        if(symbolFound == nullptr){
          //add to symbol table
          auto newSymTable =  symbol_stack.back();
          if(newSymTable == nullptr)
            newSymTable = new symbol_table();
          symbol_entry entry(node->lexinfo, newSymbol);
          newSymTable->insert(entry);        
        }
        else{
          //don't add but update if it's already there
          //error
        }
      }
      break;

    case TOK_PROTOTYPE: {
        newSymbol->attributes[ATTR_function]=true;
        newSymbol->blocknum = 0;
        symbol* symbolFound = nullptr;

        //type check in block 0 of stack if 
        //there is a prototype for the function declared already

        symbolFound = traverse_block(*symbol_stack.back(), tname);
     
         /*
        for (size_t child = 0; child < node->children.size();
            ++child) {
          symbol* newChildSymbol = new symbol();
          auto tname = get_yytname (node->children[child]->symbol);
          if (strstr (tname, "TOK_") == tname) tname += 4;
          auto newSymParams = newSymbol->parameters;
          newSymParams->push_back(newChildSymbol);
        } */

        if(symbolFound == nullptr){
          //add to symbol table
          auto newSymTable = symbol_stack.back();
          if(newSymTable == nullptr)
            newSymTable = new symbol_table();
          printf("prototype test: 3\n");

          symbol_entry entry(node->lexinfo, newSymbol);
          printf("prototype test: 4\n");

          newSymTable->insert(entry);
          printf("prototype test: 5\n");
        }
        else{
          //don't add but update if it's already there
          //error
        }
      }
      break;

    case TOK_FUNCTION: {
        newSymbol->attributes.set(ATTR_function);
        // newSymbol->blocknum = 0;
        symbol* symbolFound = nullptr;

        //type check in block 0 of stack if 
        //there is a prototype for the function declared already
        symbolFound = traverse_block(*symbol_stack.back(), tname);
    
        /*
        for (size_t child = 0; child < node->children.size();
            ++child) {
          newSymbol->blocknum = 0;
          symbol* newChildSymbol = new symbol();
          symbol_entry entry(node->lexinfo, newChildSymbol);        
          newSymbol->fields = new symbol_table();
          newSymbol->fields->insert(entry);
        } 

        for (size_t child = 0; child < node->children.size();
            ++child) {
          symbol* newChildSymbol = new symbol();
          auto tname = get_yytname (node->children[child]->symbol);
          if (strstr (tname, "TOK_") == tname) tname += 4;
          auto newSymParams = newSymbol->parameters;
          newSymParams->push_back(newChildSymbol);
        } 
        */

        if(symbolFound == nullptr){
          //add to symbol table
          auto newSymTable = symbol_stack.back();
          if(newSymTable == nullptr)
            newSymTable = new symbol_table();
          symbol_entry entry(node->lexinfo, newSymbol);
          newSymTable->insert(entry);        
        }
        else{
          //don't add but update if it's already there
          //error because function is already there
        }
      }
      break;

    case TOK_PARAMLIST: {
        newSymbol->attributes.set(ATTR_param);
        newSymbol->blocknum = next_block;
        newSymbol->parameters = nullptr;
    
        /*
        for (size_t child = 0; child < node->children.size();
            ++child) {
          symbol* newChildSymbol = new symbol();
          symbol_entry entry(node->lexinfo, newChildSymbol);        
          newSymbol->fields = new symbol_table();
          newSymbol->fields->insert(entry);
        } 
        */

        //add to symbol table
        auto newSymTable =  symbol_stack.back();
        if(newSymTable == nullptr)
          newSymTable = new symbol_table();
        symbol_entry entry(node->lexinfo, newSymbol);
        newSymTable->insert(entry);           
      }
      break;

    default: printf("newSymbol wasn't anything accounted for \n");
  }

  return nullptr;
}



static void symboltable_print(FILE* outfile, symbol_table* symtable){

  //print after all children have 
  const char* tname;
  for (auto sym = symtable->begin(); sym != symtable->end(); ++sym){
    tname = sym->first->c_str();

    fprintf (outfile, "%s (%ld.%ld.%ld) {%ld}",
        tname, sym->second->filenr, sym->second->linenr,
        sym->second->offset,sym->second->blocknum);

    if(sym->second->attributes.test(ATTR_void)) fprintf(outfile, " void");
    if(sym->second->attributes.test(ATTR_bool)) fprintf(outfile, " bool");
    if(sym->second->attributes.test(ATTR_char)) fprintf(outfile, " char");
    if(sym->second->attributes.test(ATTR_int)) fprintf(outfile, " int");
    if(sym->second->attributes.test(ATTR_null)) fprintf(outfile, " null");
    if(sym->second->attributes.test(ATTR_string)) fprintf(outfile, " string");
    if(sym->second->attributes.test(ATTR_struct)) fprintf(outfile, " struct");
    if(sym->second->attributes.test(ATTR_array)) fprintf(outfile, " array");
    if(sym->second->attributes.test(ATTR_function)) fprintf(outfile, " function");
    if(sym->second->attributes.test(ATTR_variable)) fprintf(outfile, " variable");
    if(sym->second->attributes.test(ATTR_field)) fprintf(outfile, " field");
    if(sym->second->attributes.test(ATTR_typeid)) fprintf(outfile, " typeid");
    if(sym->second->attributes.test(ATTR_param)) fprintf(outfile, " param");
    if(sym->second->attributes.test(ATTR_lval)) fprintf(outfile, " lval");
    if(sym->second->attributes.test(ATTR_const)) fprintf(outfile, " const");
    if(sym->second->attributes.test(ATTR_vreg)) fprintf(outfile, " vreg");
    if(sym->second->attributes.test(ATTR_vaddr)) fprintf(outfile, " vaddr");

    fprintf (outfile, "\n");
  }
}


static void dump_symboltable_rec (FILE* outfile, astree* node) {
  // dump node
  dump_symbol (node);

  // print symbol table
  //type_check(root, *symbol_stack.back());
  symboltable_print(outfile, symbol_stack.back());

  // recursively calls for each child 
  for (size_t child = 0; child < node->children.size();
      ++child) {
    symbol_stack.push_back(nullptr);
    next_block++;
    dump_symboltable_rec(outfile, node->children[child]); 
    next_block--;
    symbol_stack.pop_back();
  }

  //delete root;
}


void dump_symboltable (FILE* outfile, astree* root) {
   if (root == NULL) return;
   dump_symboltable_rec (outfile, root);
   fflush (NULL);
}


string type_check (astree* node, symbol_table table) {
   string type = "";
   int success = 0;
   string left = "", right = "", child = "", lex = "";
   int decl = 0;

   switch (node->symbol) {
      /** While and If/Else */
      case TOK_WHILE: case TOK_IF: case TOK_IFELSE:
         child = type_check(node->children.at(0), table);
         if (child != "bool") { success = 1; break; }
         break;

      /** Allocator */
      case TOK_NEW:
         lex = *(node->children.at(0)->lexinfo);
         type = traverse_block_two(table, lex);
         break;

      case TOK_NEWSTRING:
         if (type_check(node->children.at(0), table) != "int") {
            success = 1;
            break;
         }
         type = "string";
         break;

      case TOK_NEWARRAY:
         decl = node->children.at(0)->symbol;
         if (decl != TOK_INT && decl != TOK_CHAR && decl != TOK_BOOL &&
             decl != TOK_STRING && decl != TOK_TYPEID) { success = 1; break; }
         child = type_check (node->children.at(1), table);
         if (child != "int") { success = 1; break; }
         lex = *(node->children.at(1)->lexinfo);
         type = traverse_block_two(table, lex);
         break;

      /** Ident */
      case TOK_IDENT: 
         lex = *(node->lexinfo);
         type = traverse_block_two(table, lex); 
         break;
   }

   if (success == 1) {
      printf("ERROR\n");
      //fprintf(stderr, "file number :::error: PUT ERROR MESSAGE HERE ON TYPE CHECKING\n");
   }
   return type;
}

tok convert (string str) {
  if (str == "ROOT") return ROOT;
  else if (str == "VOID") return VOID;
  else if (str == "NULL") return NUL;
  else if (str == "ORD") return ORD;
  else if (str == "CHR") return CHR;
  else if (str == "ARRAY") return ARRAY;
  else if (str == "RETURN") return RETURN;
  else if (str == "CALL") return CALL;
  else if (str == "BOOL") return BOOL;
  else if (str == "CHAR") return CHAR;
  else if (str == "INT") return INT;
  else if (str == "STRING") return STRING;
  else if (str == "FIELD") return FIELD;
  else if (str == "VARDECL") return VARDECL;
  else if (str == "IDENT") return IDENT;
  else if (str == "DECLID") return DECLID;
  else if (str == "FIELDS") return FIELDS;
  else if (str == "TYPEID") return TYPEID;
  else if (str == "PROTOTYPE") return PROTOTYPE;
  else if (str == "FUNCTION") return FUNCTION;
  else if (str == "PARAMLIST") return PARAMLIST;
  else return NONE;
}

