#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <libgen.h>
#include <string.h>

#include "auxlib.h"
#include "stringset.h"

using namespace std;

const string CPP = "/usr/bin/cpp ";
const size_t LINESIZE = 1024;

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
        printf ("%s:line %d: [%s]\n", filename, linenr, buffer);
        // http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
        int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                                &linenr, filename);
        if (sscanf_rc == 2) {
            printf ("DIRECTIVE: line %d file \"%s\"\n", linenr, filename);
            continue;
        }
        char* savepos = NULL;
        char* bufptr = buffer;
        for (int tokenct = 1;; ++tokenct) {
            char* token = strtok_r (bufptr, " \t\n", &savepos);
            bufptr = NULL;
            if (token == NULL) break;
            intern_stringset(token);
            printf ("token %d.%d: [%s]\n", linenr, tokenct, token);
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
    int yy_flex_debug = 0, yy_debug = 0, option = -1;
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
      	        yy_debug = 1;
                yy_debug = yy_debug;
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
        string command = CPP + " " + d_string.c_str() + filename;
        printf ("command=\"%s\"\n", command.c_str());
        FILE* pipe = popen (command.c_str(), "r");
        if (pipe == NULL)
        {
            syserrprintf (command.c_str());
        }
        else
        {
            cpplines (pipe, filename);
            int pclose_rc = pclose (pipe);
            eprint_status (command.c_str(), pclose_rc);
            string file = basename(filename);
            ofstream outfile (file.substr(0, file.find(".")) + ".str");
            dump_stringset(outfile);
            outfile.close();
        }
        counter++;
    }
    return get_exitstatus();
}
