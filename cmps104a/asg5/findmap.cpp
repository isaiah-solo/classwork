// $Id: findmap.cpp,v 1.4 2014-11-20 16:14:03-08 - - $

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {
   unordered_map<string,int> um;
   for (const string& s: vector<string> {"foo", "bar", "baz"}) {
      const auto& e = um.find (s);
      cout << s << ": ";
      if (e == um.end()) cout << "not found";
                    else cout << e->second;
      cout << endl;
   }
   int i{};
   for (const string& s: vector<string> {"foo", "bar", "baz"}) {
      um.insert ({s, ++i});
   }
   for (const auto& i: um) {
      cout << i.first << ": " << i.second << endl;
   }
   return 0;
}
