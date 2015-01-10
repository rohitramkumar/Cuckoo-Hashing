#include <iostream>
#include <string>
#include <time.h>
#include <map>
#include "dictionary.h"

using namespace std;


int main( int argc, const char* argv[] )
{
    /*
  time_t start;
  time(&start);
  int count = 1000000;
  bool mine = true;

  if (mine) {
    Dictionary<string, string> dict;
    for(int i = 0; i < count; i++ ) {
      dict.insert("a" + std::to_string(i), "b" + std::to_string(i));
    }
  } else {
    std::map<string, string> dict;
    for(int i = 0; i < count; i++ ) {
      dict.insert({"a" + std::to_string(i), "b" + std::to_string(i)});
    }
  }

  time_t end;
  double seconds;
  time(&end);
  seconds = difftime(end, start);
  cout << "time taken:" << seconds << endl;

  //dict.printTables();

   for(int i = 0; i < count; i++) {
   cout << (dict.find("a" + std::to_string(i))) << '\n';
   }
   */

    Dictionary<string, int>* d1 = new Dictionary<string, int>;
    d1->insert("a", 1);
    d1->insert("b", 2);
    d1->insert("c", 3);

    Dictionary<string, int>* d2 = new Dictionary<string, int>(*d1);
    delete d1;

    cout << d2->find("a") << endl;
    cout << d2->find("b") << endl;
    cout << d2->find("c") << endl;
    delete d2;

  return 0;
  
}
