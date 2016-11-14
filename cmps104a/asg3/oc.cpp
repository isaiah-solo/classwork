#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <libgen.h>
#include <string.h>

#include "auxlib.h"
#include "stringset.h"
#include "lyutils.h"
#include "astree.h"

using namespace std;

const string CPP = "/usr/bin/cpp ";
const size_t LINESIZE = 1024;
const string STR = ".str";
const string TOK = ".tok";
const string AST = ".ast";

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}
// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
    int linenr = 1;
    char inputname[LINESIZE];
    strcpy (inputname, filename);
    for (;;) {
        char buffer[LINESIZE];
        char* fgets_rc = fgets (buffer, LINESIZE, pipe);
        if (fgets_rc == NULL) break;
        chomp (buffer, '\n');
        int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                                &linenr, filename);
        if (sscanf_rc == 2) {
            continue;
        }
        char* savepos = NULL;
        char* bufptr = buffer;
        for (int tokenct = 1;; ++tokenct) {
            char* token = strtok_r (bufptr, " \t\n", &savepos);
            bufptr = NULL;
            if (token == NULL) break;
            intern_stringset(token);
        }
        ++linenr;
    }
}

int main (int argc, char** argv)
{
    // check if executable name provided
    if (argc < 2) {
        cerr << "Expected argument" << endl;
        return 1;
    }

    // set executable name
    set_execname (argv[0]);

    // check for options
    int option = -1;
    yy_flex_debug = 0; yydebug = 0;
    string at_flags = "", d_string = "";

    while ((option = getopt (argc, argv, "ly@:D:")) != -1)
    {
        switch (option)
        {
            case 'l':
                yy_flex_debug = 1;
                yy_flex_debug = yy_flex_debug;
                    break;
            case 'y':
                yydebug = 1;
                yydebug = yydebug;
                break;
            case '@':
                at_flags = optarg;
                set_debugflags(at_flags.c_str());
                break;
            case 'D':
                d_string = string("-D ") + optarg + " ";
                break;
            default:
                return 1;
        }
    }

    // check if arguments included after options
    if (optind >= argc) {
        cerr << "Expected argument after options" << endl;
        return 1;
    }

    // create vector that holds file names
    vector<string> v = {};
    for (int i = optind; i < argc; ++i) {
        v.push_back(string(argv[i]));
    }

    // read files one by one
    int counter = 0;
    for (auto itor = v.begin(); itor != v.end(); ++itor)
    {
        char* filename = (char*) itor->c_str();
        string base = basename(filename);
        string base_name = base.substr(0, base.find("."));
        string command = CPP + " " + d_string.c_str() + filename;
        yyin = popen (command.c_str(), "r");
        if (yyin == NULL)
        {
            syserrprintf (command.c_str());
        }
        else
        {
            cpplines (yyin, filename);
            yyin = popen (command.c_str(), "r");

            // tok file
            output_file_tok = fopen((base_name + TOK).c_str(), "w");
            for (int token = yylex(); token != YYEOF; token = yylex()) {
                yyprint(output_file_tok, token, yylval);
            }
            fclose(output_file_tok);

            // str file
            ofstream str_file (base_name + STR);
            dump_stringset(str_file);
            str_file.close();

            // ast file
            yyin = popen (command.c_str(), "r");
            yyparse();
            output_file_tok = fopen((base_name + AST).c_str(), "w");
            dump_astree (output_file_tok, yyparse_astree);
            fclose(output_file_tok);

            int pclose_rc = pclose (yyin);
            eprint_status (command.c_str(), pclose_rc);
        }
        counter++;
    }

    return get_exitstatus();
}
