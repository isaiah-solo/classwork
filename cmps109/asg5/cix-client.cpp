// $Id: cix-client.cpp,v 1.7 2014-07-25 12:12:51-07 - - $

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "cix_protocol.h"
#include "logstream.h"
#include "signal_action.h"
#include "sockets.h"

logstream log (cout);

unordered_map<string,cix_command> command_map {
   {"exit", CIX_EXIT},
   {"get" , CIX_GET },
   {"help", CIX_HELP},
   {"ls"  , CIX_LS  },
   {"put" , CIX_PUT },
   {"rm"  , CIX_RM  },
};

void cix_get (string filename, client_socket& server) {
   cix_header header;
   header.cix_command = CIX_GET;
   for (unsigned int i = 0; i < filename.size(); ++i)
      header.cix_filename[i] = filename.at(i);
   header.cix_nbytes = 0;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.cix_command != CIX_FILE) {
      log << "sent CIX_GET, server did not return CIX_ACK" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.cix_nbytes];
      cout << "testing" << endl;
      recv_packet (server, buffer, header.cix_nbytes);
      log << "received " << header.cix_nbytes << " bytes" << endl;
      cout << buffer;
      ofstream new_file (header.cix_filename);
      if (not new_file.fail())
      {
         new_file << buffer;
         new_file.close();
      }
   }
}

void cix_help() {
   static vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.cix_command = CIX_LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.cix_command != CIX_LSOUT) {
      log << "sent CIX_LS, server did not return CIX_LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.cix_nbytes + 1];
      recv_packet (server, buffer, header.cix_nbytes);
      log << "received " << header.cix_nbytes << " bytes" << endl;
      buffer[header.cix_nbytes] = '\0';
      cout << buffer;
   }
}

void cix_put (string filename, client_socket& server) {
   cix_header header;
   header.cix_command = CIX_PUT;
   for (unsigned int i = 0; i < filename.size(); ++i)
      header.cix_filename[i] = filename.at(i);
   string lines = "", line = "";
   ifstream file (filename);
   if (file.good())
   {
      while (not file.eof())
      {
         getline(file, line);
         lines += line + "\n";
      }
      file.close();
   }
   char contents[lines.size() + 1];
   for (unsigned int i = 0; i < lines.size(); ++i)
      contents[i] = lines.at(i);
   contents[lines.size()] = '\0';
   if (lines.size() < 1)
      header.cix_nbytes = 0;
   else
      header.cix_nbytes = sizeof contents;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.cix_command != CIX_ACK) {
      if (file.is_open())
         file.close();
      log << "sent CIX_PUT, server did not return CIX_ACK" << endl;
      log << "server returned " << header << endl;
   }else {
      send_packet (server, contents, header.cix_nbytes);
      log << "received " << header.cix_nbytes << " bytes" << endl;
   }
}

void cix_rm (string filename, client_socket& server) {
   cix_header header;
   for (unsigned int i = 0; i < filename.size(); ++i)
      header.cix_filename[i] = filename.at(i);
   header.cix_command = CIX_RM;
   header.cix_nbytes = 0;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.cix_command != CIX_ACK) {
      log << "sent CIX_RM, server did not return CIX_ACK" << endl;
      log << "server returned " << header << endl;
   }
   else
      log << "received " << header.cix_nbytes << " bytes" << endl;
}


void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

bool SIGINT_throw_cix_exit {false};
void signal_handler (int signal) {
   log << "signal_handler: caught " << strsignal (signal) << endl;
   switch (signal) {
      case SIGINT: case SIGTERM: SIGINT_throw_cix_exit = true; break;
      default: break;
   }
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   signal_action (SIGINT, signal_handler);
   signal_action (SIGTERM, signal_handler);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line, command = "", filename = "";
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         if (SIGINT_throw_cix_exit) throw cix_exit();
         int counter = 1;
         for (auto itor = line.begin(); itor != line.end(); ++itor, ++counter)
         {
            if (*itor == ' ')
            {
               filename = line.substr(counter);
               break;
            }
            command += *itor;
         }
         log << "command " << command << endl;
         const auto& itor = command_map.find (command);
         cix_command cmd = itor == command_map.end()
                         ? CIX_ERROR : itor->second;
         switch (cmd) {
            case CIX_EXIT:
               throw cix_exit();
               break;
            case CIX_GET:
               cix_get(filename, server);
               break;
            case CIX_HELP:
               cix_help();
               break;
            case CIX_LS:
               cix_ls (server);
               break;
            case CIX_PUT:
               cix_put(filename, server);
               break;
            case CIX_RM:
               cix_rm(filename, server);
               break;
            default:
               log << command << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

