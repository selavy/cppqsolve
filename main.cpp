#include <iostream>
#include "Config.h"
#include "Database.hpp"

using namespace std;

int main( int argc, char **argv ) {
  Database db;
  cout << argv[0] << " " << _VERSION_MAJOR << "." << _VERSION_MINOR << endl;
  return 0;
}
