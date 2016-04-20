// $Id: main.cpp,v 1.6 2014-07-09 11:50:34-07 - - $

//
// Isaiah Solomon
// 1344680
// icsolomo
// Mackey
// CS 109
//
// Assignment 3
//

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_pair = xpair<const string,string>;
using str_str_map = listmap<string,string>;

void scan_options (int argc, char** argv)
{
   opterr = 0;
   for (;;)
   {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option)
      {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

void do_operation (str_str_map& the_map, const string& key, 
                   const string& value, const string& input, 
                   const size_t& equals)
{
   if (input.size() and input.at(0) == '#') // #
   {
      if (input.size() > 1)
         cout << input.substr(1, input.size());
      cout << endl;
   }
   else if (key.size() and equals >= 
            input.size() and not value.size()) // key
   {
      str_str_pair temp {key, value};
      bool found = true;
      auto pointer = the_map.find(key, found);
      if (found == true)
         cout << *pointer << endl;
      else
         cout << key << ": key not found" << endl;
   }
   else if (key.size() and equals < input.size() 
            and not value.size()) // key =
   {
      the_map.erase(key);
   }
   else if (key.size() and equals < input.size() 
            and value.size()) // key = value
   {
      str_str_pair temp {key, value};
      auto pointer = the_map.insert(temp);
      cout << *pointer << endl;
   }
   else if (not key.size() and equals < input.size()
            and not value.size()) // =
   {
      the_map.print();
   }
   else if (not key.size() and equals < input.size()
            and value.size()) // = value
   {
      the_map.value_print(value);
   }
}

void trim (string& key, string& value, const string& input, 
           size_t& equals)
{
   equals = input.find_first_of('=', 0);
   key = input.substr(0, equals);
   if (equals < input.size())
      value = input.substr(equals + 1, input.size());
   while (key.size() and key.at(0) == ' ')
      key.erase(0, 1);
   while (key.size() and key.at(key.size() - 1) == ' ')
      key.erase(key.size() - 1, 1);
   while (value.size() and value.at(0) == ' ')
      value.erase(0, 1);
   while (value.size() and value.at(value.size() - 1) == ' ')
      value.erase(key.size() - 1, 1);
}

int main (int argc, char** argv)
{
   //sys_info::set_execname (argv[0]);
   scan_options (argc, argv);

   str_str_map the_map {};
   string input = "", key = "", value = "";
   size_t equals = 0;
   int counter = 1;

   for (int i = 1; i < argc; ++i)
   {
      ifstream the_file (argv[i]);
      if (not the_file.is_open())
         continue;
      else
      {
         while (getline(the_file, input))
         {
            cout << argv[i] << ": " << counter << ": " << input << endl;
            trim(key, value, input, equals);
            do_operation(the_map, key, value, input, equals);
            ++counter;
         }
      }
      the_file.close();
      counter = 1;
   }

   while (argc == 1)
   {
      input = ""; key = ""; value = "";
      getline(cin, input);
      if (cin.eof())
      {
         the_map.erase_all();
         break;
      }
      cout << "-: " << counter << ": " << input << endl;
      trim(key, value, input, equals);
      do_operation(the_map, key, value, input, equals);
      counter += 1;
   }

   return EXIT_SUCCESS;
}

