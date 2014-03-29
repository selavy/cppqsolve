#include <iostream>
#include <cstring>
#include <string>
#include "Config.h"
#include "Database.hpp"
#include "OrderEngine.hpp"
#include "boost/version.hpp"
#include "boost/program_options.hpp"

using namespace std;

void print_version( char * program_name );
void print_license();
void print_boost_version();

int main( int argc, char **argv ) {
  boost::program_options::options_description desc( "Allowed options" );
  desc.add_options()
    ( "help,h", "produce help message" )
    ( "version,v", "print version" )
    ( "start,s", boost::program_options::value<string>(), "start date for simulation" )
    ( "end,e", boost::program_options::value<string>(), "end date for simulation" )
    ( "strategy,S", boost::program_options::value<string>(), "python strategy file" )
    ;

  boost::program_options::variables_map vm;
  boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc), vm );
  boost::program_options::notify( vm );

  if( vm.count( "help" ) ) {
    cout << desc << endl;
    return 0;
  }

  if( vm.count( "version" ) ) {
    print_version( argv[0] );
    return 0;
  }

  if( vm.count( "start" ) ) {
    cout << "Start date set to "
	 << vm["start"].as<string>()
	 << endl;
  } else {
    cout << "Must provide a start date for simulation." << endl;
  }

  if( vm.count( "end" ) ) {
    cout << "End date set to "
	 << vm["end"].as<string>()
	 << endl;
  } else {
    cout << "No end date given.  Using current date." << endl;
  }

  if( vm.count( "strategy" ) ) {
    cout << "Strategy file set to "
	 << vm["strategy"].as<string>()
	 << endl;
  } else {
    cout << "Must provide a strategy file." << endl;
  }

  return 0;
}

void print_version( char * program_name ) {
  cout << program_name << " (QSolve) " << _VERSION_MAJOR << "." << _VERSION_MINOR << endl;
  print_license();
  cout << "\nWritten by " << _AUTHORS << endl;
}

void print_boost_version() {
  cout << "Using Boost libs version: " << BOOST_VERSION / 100000 << "."
       << BOOST_VERSION / 100 % 1000 << "."
       << BOOST_VERSION % 100 << endl;
}

void print_license() {
  cout << "Copyright (C) 2014 Grimberg, Hale, Polsley, Lesslie."
       << endl
       << "This is free software: you are free to change and redistribute it."
       << endl
       << "There is NO WARRANTY, to the extent permitted by law."
       << endl;
}
