#include <iostream>
#include "Config.h"
#include "Database.hpp"
#include "OrderEngine.hpp"
#include "boost/version.hpp"

using namespace std;

int main( int argc, char **argv ) {
  Database db;
  OrderEngine orderEngine;
  cout << argv[0] << " " << _VERSION_MAJOR << "." << _VERSION_MINOR << endl;
  cout << "Using Boost libs version: " << BOOST_VERSION / 100000 << "."
       << BOOST_VERSION / 100 % 1000 << "."
       << BOOST_VERSION % 100 << endl;
  return 0;
}
