#include <iostream>
#include <string>
#include "Config.h"
#include "Database.hpp"
#include "OrderEngine.hpp"
#include "boost/program_options.hpp"

using namespace std;

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


