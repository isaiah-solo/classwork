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

static symbol* traverse_block(symbol_table mymap,string tname){

  symbol_table::iterator got = mymap.find (&tname);

  //found symbol
  if ( got != mymap.end() )
    return got->second;

  return traverse_block(*got->second->fields,tname);
}

static string traverse_block_two(symbol_table mymap,string tname){

  symbol_table::iterator got = mymap.find (&tname);

  //found symbol
  if ( got != mymap.end() ) {
    string type = *(got->first);
    return type;
  }

  return traverse_block_two(*got->second->fields,tname);
}



static void dump_symbol (astree* node, int depth) {

  auto tname = get_yytname (node->symbol);
  if (strstr (tname, "TOK_") == tname) tname += 4;

  printf("the symbol is: %s\n the lexinfo is: %s\n",tname,node->lexinfo->c_str());
  
  symbol* newSymbol = new symbol();
  newSymbol->filenr = node->filenr; 
  newSymbol->linenr = node->linenr;       
  newSymbol->offset = node->offset; 

  if(strcmp(tname,"ROOT")==0){
    newSymbol->blocknum = 0;
    /*
    auto newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    */
  }
  else if(strcmp(tname,"VOID") == 0){
    newSymbol->attributes[ATTR_void]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    newSymbol->blocknum = 0;

    auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    //type checking if it is the return value of function, error otherwise
    //check if children containes function or type_id  
    //may only be used as the return type of a function. It is an error to declare variables, parameters, or fields of this type.  

    //fprintf(stderr, "file number %ld:%ld:%ld:error: PUT ERROR MESSAGE HERE ON TYPE CHECKING\n",newSymbol->filenr,newSymbol->linenr,newSymbol->offset);

  }
  else if(strcmp(tname,"NULL")==0){
    newSymbol->attributes[ATTR_null]=true;
    newSymbol->attributes[ATTR_const]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    newSymbol->blocknum = next_block;

    auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 

    //type check if it is a constant, and only if its a constant
    //null : has the single constant null. The syntax prevents it from being used in a type declaration.

  }
  else if(strcmp(tname,"ORD")==0){
    newSymbol->attributes[ATTR_vreg]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    newSymbol->blocknum = 0;

    auto newSymTable =  symbol_stack.back();
    if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    //type check if 'ord' char → int vreg

  }
  else if(strcmp(tname,"CHR")==0){
    newSymbol->attributes[ATTR_vreg]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    newSymbol->blocknum = 0;

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    //type check if ‘chr’ int → char vreg
  }
  else if(strcmp(tname,"ARRAY") == 0){
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
  else if(strcmp(tname, "RETURN") == 0){
    newSymbol->attributes[ATTR_vaddr]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    symbol* symbolFound = nullptr;

    symbolFound = traverse_block(*symbol_stack.back(), tname);

    // type check if ‘return’ compatible → 

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 

  }
  else if(strcmp(tname, "CALL") == 0){
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
  else if(strcmp(tname,"BOOL") ==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_bool]=true;
    newSymbol->attributes[ATTR_const]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 

    //type check:  bool : has constants false and true, and is an 8-bit byte.

  }
  else if(strcmp(tname,"CHAR")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_char]=true;
    newSymbol->attributes[ATTR_const]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    //type check : 
    //char: isan8-bitbyte. Valuesbetween0x00and0x7FareASCIIcharacters, and values between 0x80 and 0xFF are locale dependent.
  }
  else if(strcmp(tname,"INT")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_int]=true;
    newSymbol->attributes[ATTR_const]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    //type check:   is a signed two’s complement integer.
  }
  else if(strcmp(tname,"STRING")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_string]=true;
    newSymbol->attributes[ATTR_const]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
    symbol_entry entry(node->lexinfo, newSymbol);
    newSymTable->insert(entry);
    symbol_stack.push_back(newSymTable); 
    
    //type check: string: is effectivly an array of characters
    // and has string constants associ- ated with it. Its size 
    //is fixed when allocated. The length of the string con- tained in
    // the array varies up to the maximum allocated length, depending on 
    //where the null plug (’\0’) is placed. Strings are stored in the same for- mat as in C.
  }
  else if(strcmp(tname, "FIELD")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_field]=true;
    newSymbol->attributes[ATTR_lval]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    symbol* symbolFound = nullptr;

    //type check:
    //Field names are identifiers which may only be 
    //used immedi- ately following the dot (.) operator for field selection.

    symbolFound = traverse_block(*symbol_stack.back(), tname);


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
  else if(strcmp(tname,"VARDECL")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_field]=true;
    newSymbol->attributes[ATTR_lval]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    symbol* symbolFound = nullptr;


    //Whenever you see a variable declaration, 
    //look it up in the symbol table. Print a duplicate declaration 
    //error message if it is in the symbol table at the top of the symbol
    // vector stack.
    symbolFound = traverse_block(*symbol_stack.back(), tname);

    if(symbolFound == nullptr){
      //add to symbol table
      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
      symbol_entry entry(node->lexinfo, newSymbol);
      newSymTable->insert(entry);        
          }
    else{
      symbolFound = newSymbol;    
    }

  }
  else if(strcmp(tname,"IDENT")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_field]=true;
    newSymbol->attributes[ATTR_lval]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    symbol* symbolFound = nullptr;


    //Whenever you see a variable declaration, 
    //look it up in the symbol table. Print a duplicate declaration 
    //error message if it is in the symbol table at the top of the symbol
    // vector stack.
    symbolFound = traverse_block(*symbol_stack.back(), tname);

    if(symbolFound == nullptr){
      //add to symbol table
      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
      symbol_entry entry(node->lexinfo, newSymbol);
      newSymTable->insert(entry);        
          }
    else{
      symbolFound = newSymbol;
    }

  }
  else if(strcmp(tname,"DECLID")==0){
    newSymbol->blocknum = next_block;
    newSymbol->attributes[ATTR_function]=true;
    newSymbol->fields = nullptr;
    newSymbol->parameters = nullptr;
    symbol* symbolFound = nullptr;
    
    printf("declid: test 0\n");
    if(symbol_stack.back() != nullptr )
      symbolFound = traverse_block(*symbol_stack.back(), tname);
    printf("declid: test 1\n");

    if(symbolFound == nullptr){
      //add to symbol table
          printf("declid: test 2\n");

      auto newSymTable =  symbol_stack.back();
      if(newSymTable == nullptr)
        newSymTable = new symbol_table();
          printf("declid: test 3\n");

      symbol_entry entry(node->lexinfo, newSymbol);
          printf("declid: test 4\n");

      newSymTable->insert(entry);        
          printf("declid: test 5\n");

    }
    else{
      //not the same type
      //error
          printf("declid: test 6\n");


    }

  }
  else if(strcmp(tname,"FIELDS")==0){
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
    } */

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
  else if(strcmp(tname,"TYPEID")==0){
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
  else if(strcmp(tname,"PROTOTYPE")==0){
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
  else if(strcmp(tname,"FUNCTION")==0){
    newSymbol->attributes[ATTR_function]=true;
    newSymbol->blocknum = 0;
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
  else if(strcmp(tname,"PARAMLIST")==0){
    newSymbol->attributes[ATTR_param]=true;
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
  else{
      printf("newSymbol wasn't anything accounted for \n");
  }
 
}



static void dump_symboltable_rec_helper(FILE* outfile, symbol_table* topSymTable){

  //print after all children have 
  const char* tname;
  for ( auto x = topSymTable->begin(); x != topSymTable->end(); ++x ){
      //tname = get_yytname (x.first);
      //if (strstr (tname, "TOK_") == tname) tname += 4;
      tname = x->first->c_str();
    
    //tabLinesSym += "    ";
    //if ( strcmp(tname,"ROOT") != 0) fprintf (outfile, tabLinesSym.c_str());
    fprintf (outfile, "%s (%ld.%ld.%ld) {%ld}",
        tname,x->second->filenr, x->second->linenr,
        x->second->offset,x->second->blocknum);
    if(x->second->attributes[ATTR_void]==true)
      fprintf(outfile, " void");
    if(x->second->attributes[ATTR_bool]==true)
      fprintf(outfile, " bool");
    if(x->second->attributes[ATTR_char]==true)
      fprintf(outfile, " char");
    if(x->second->attributes[ATTR_int]==true)
      fprintf(outfile, " int");
    if(x->second->attributes[ATTR_null]==true)
      fprintf(outfile, " null");
    if(x->second->attributes[ATTR_string]==true)
      fprintf(outfile, " string");
    if(x->second->attributes[ATTR_struct]==true)
      fprintf(outfile, " struct");
    if(x->second->attributes[ATTR_array]==true)
      fprintf(outfile, " array");
    if(x->second->attributes[ATTR_function]==true)
      fprintf(outfile, " function");
    if(x->second->attributes[ATTR_variable]==true)
      fprintf(outfile, " variable");
    if(x->second->attributes[ATTR_field]==true)
      fprintf(outfile, " field");
    if(x->second->attributes[ATTR_typeid]==true)
      fprintf(outfile, " typeid");
    if(x->second->attributes[ATTR_param]==true)
      fprintf(outfile, " param");
    if(x->second->attributes[ATTR_lval]==true)
      fprintf(outfile, " lval");
    if(x->second->attributes[ATTR_const]==true)
      fprintf(outfile, " const");
    if(x->second->attributes[ATTR_vreg]==true)
      fprintf(outfile, " vreg");
    if(x->second->attributes[ATTR_vaddr]==true)
      fprintf(outfile, " vaddr");
    fprintf (outfile, "\n");
  }

}


static void dump_symboltable_rec (FILE* outfile, astree* root,
                             int depth) {
  if (root == NULL) return;
  //fprintf (outfile, "%*s%s ", depth * 3, "",
  //         root->lexinfo->c_str());
  dump_symbol (root, depth);
  fprintf (outfile, "\n");
  for (size_t child = 0; child < root->children.size();
      ++child) {
    //print after all children have 
    symbol_stack.push_back(nullptr);
    next_block++;
    //print here
    dump_symboltable_rec_helper(outfile, symbol_stack.back()); 
    //tabLinesSym.erase(tabLinesSym.end()-4,tabLinesSym.end());
    next_block--;
    symbol_stack.pop_back();
  }
  printf("TEST3\n");

  type_check(root, *symbol_stack.back());
   printf("TEST4\n"); 
  dump_symboltable_rec_helper(outfile, symbol_stack.back());
  //tabLinesSym.erase(tabLinesSym.end()-4,tabLinesSym.end());
    printf("TEST5\n");

  next_block--;
  symbol_stack.pop_back();
    printf("TEST6\n");


   // dump_symboltable_rec (outfile, root->children[child],depth + 1);
  //delete root;

}

void dump_symboltable (FILE* outfile, astree* root) {
   dump_symboltable_rec (outfile, root, 0);
   fflush (NULL);
}

string type_check (astree* node, symbol_table table) {
   string type = "";
   int success = 0;
   string left = "", right = "", child = "", lex = "";
   int decl = 0, id = 0;

   switch (node->symbol) {

      /** Binary/Unary Operations */
      case '+': case '-': case '*': case'/': case '%': case '!': case TOK_ORD: case TOK_CHR:
         // Check if Binop
         if (node->children.size() == 2) {
            // Set Operands left and right
            left = type_check(node->children.at(0), table);
            right = type_check(node->children.at(1), table);
            if (left != "int" || right != "int") { success = 1; break; }
            type = "int";
         }

         // Check if Unop
         else if (node->children.size() == 1) {
            switch (node->symbol) {
               case '+': case '-':
                  child = type_check(node->children.at(0), table);
                  if (child != "int") { success = 1; break; }
                  type = "int";
                  break;
               case '!':
                  child = type_check(node->children.at(0), table);
                  if (child != "bool") { success = 1; break; }
                  type = "bool";
                  break;
               case TOK_ORD:
                  child = type_check(node->children.at(0), table);
                  if (child != "char") { success = 1; break; }
                  type = "int";
                  break;
               case TOK_CHR:
                  child = type_check(node->children.at(0), table);
                  if (child != "int") { success = 1; break; }
                  type = "char";
                  break;
               default: success = 1;
            }
         }
         break;

      /** Comparison Operations */
      case TOK_EQ: case TOK_NE:
         // Set Operands left and right
         left = type_check(node->children.at(0), table);
         right = type_check(node->children.at(1), table);
         if (left != right) { success = 1; break; }
         type = "bool";
         break;

      case TOK_GT: case TOK_GE: case TOK_LT: case TOK_LE:
         // Set Operands left and right
         left = type_check(node->children.at(0), table);
         right = type_check(node->children.at(1), table);
         if ((left != "int" && left != "char" && left != "bool") ||
             (right != "int" && right != "char" && right != "bool") ||
             left != right) { success = 1; break; }
         type = "bool";
         break;

      /** While and If/Else */
      case TOK_WHILE: case TOK_IF: case TOK_IFELSE:
         child = type_check(node->children.at(0), table);
         if (child != "bool") { success = 1; break; }
         break;

      /** Fields */
      case '.':
         if (node->children.at(0)->symbol != TOK_IDENT ||
             node->children.at(1)->symbol != TOK_FIELD) { success = 1; break; }
         lex = *(node->children.at(1)->lexinfo);
         type = traverse_block_two(table, lex);
         break;

      /** Vardecl */
      case TOK_VARDECL:
         decl = node->children.at(0)->symbol;
         id = node->children.at(1)->symbol;
         switch (decl) {
            case TOK_INT:
               if (id != TOK_INTCON) { success = 1; break; }
               break;
            case TOK_CHAR:
               if (id != TOK_CHARCON) { success = 1; break; }
               break;
            case TOK_BOOL:
               if (id != TOK_TRUE && id != TOK_FALSE) { success = 1; break; }
               break;
            case TOK_STRING:
               if (id != TOK_STRINGCON) { success = 1; break; }
               break;
            case TOK_TYPEID:
               if (id != TOK_TYPEID) { success = 1; break; }
               break;
            default:
               success = 1;
         }
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

      /** Constants */
      case TOK_INTCON: type = "int"; break;
      case TOK_CHARCON: type = "char"; break;
      case TOK_STRINGCON: type = "string"; break;
      case TOK_TRUE: case TOK_FALSE: type = "bool"; break;
      case TOK_NULL: type = "null"; break;

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
