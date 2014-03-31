#include "PythonInterpreter.hpp" // get rid of warning about Python.h not being first include
#include <iostream>
#include <fstream>
#include <string>
#include "QTypes.hpp"
#include "Config.h"
#include "CassandraDatabase.hpp"
#include "OrderFactory.hpp"
#include "OrderFunction.hpp"
#include "StrategyEvaluator.hpp"
#include "boost/program_options.hpp"

using namespace std;

int main( int argc, char **argv ) {
  //
  // Use Boost.Program_Options to parse the command line and configuration file input.
  // Documentation for this library available at: http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html
  //

  string config_file;
  string startDate;
  string endDate;
  string strategyFile;
  currency initialBalance;

  //
  // Generic Options are the options that can only be specified on the command line.
  //
  boost::program_options::options_description generic( "Generic options" );
  generic.add_options()
    ( "help,h", "produce help message" )
    ( "version", "print version" )
    ( "config,c", boost::program_options::value<string>(&config_file)->default_value("qsolve.cfg"), "name of a configuration file")
    ;

  //
  // Configuration options can be specified on the command line or in the configuration file.
  // The default configuration file name is "qsolve.cfg".
  //
  boost::program_options::options_description config( "Configuration options" );
  config.add_options()
    ( "start,s", boost::program_options::value<string>(&startDate), "start date for simulation" )
    ( "end,e", boost::program_options::value<string>(&endDate)->default_value("today"), "end date for simulation" )
    ( "balance,b", boost::program_options::value<currency>(&initialBalance)->default_value(0), "Initial amount of currency available" )
    ;

  //
  // Hidden options can be specified on the command line or in the configuration file, but
  // they will not be displayed in the help message because they are positional variables.
  //
  boost::program_options::options_description hidden( "Hidden Options" );
  hidden.add_options()
    ( "strategy", boost::program_options::value<string>(&strategyFile), "python strategy file" )
    ;

  //
  // cmdline_options: options searched for on the command line.
  //
  boost::program_options::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  //
  // program_options: options searched for in the configuration file.
  //
  boost::program_options::options_description config_file_options;
  config_file_options.add(config).add(hidden);
  
  //
  // visible: options that will be displayed in the help/usage message.
  //
  boost::program_options::options_description visible("Allowed options");
  visible.add(generic).add(config);

  //
  // positional arguments are ones that don't have to have a specifier before them.
  // these arguments can be identified by where they are in the command line or
  // the configuration file.
  //
  // The order of the specification on the command line determines where they are assigned.
  // The order searched for is the the order in which they are add()'d to p.
  //
  boost::program_options::positional_options_description p;
  p.add("strategy", 1);
  
  
  try {
    //
    // variables_map is a type that maps the parsed command line options to their name as specified above.
    // store( command_line_parser( ... ) ... ) parses the command line arguments.
    // store( parse_config_file( ... ) ... ) parses the configuration file if there is one.
    //
    boost::program_options::variables_map vm;

    //
    // If any of the options are not recognized, or any of the required() arguments are not provided
    // then store() will throw an exception.
    //
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
				  options(cmdline_options).positional(p).run(), vm);
    boost::program_options::notify( vm );
    
    //
    // Try to open the configuration file to parse it.
    // 
    ifstream ifs(config_file.c_str());
    if(ifs) {
      boost::program_options::store(boost::program_options::parse_config_file(ifs,config_file_options), vm);
      boost::program_options::notify(vm);
    }

    //
    // Now all parsed values should be in the variables_map vm.
    // Can check if a variable is set with vm.count( VAR_NAME ) > 0.
    //
    if( vm.count( "help" ) ) {
      cout << visible << endl;
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

    //
    // We have the command line arguments, now run the program.
    //

    //
    // Create the required componenets.
    // + PythonInterpreter
    // + Database
    // + Portfolio
    // + OrderCreator
    // + OrderEngine
    // + PortfolioDatabase
    //
    // N.B. only 1 PythonInterpreter should ever be created at a time.
    //

    //
    // Create Interpreter for the Database
    //
    PythonInterpreter interpreter( argc, argv );

    //
    // TODO: Try to move this into the StrategyEvaluation function
    // Right now it can't find the module if it is called in the StrategyEvaluator
    // construtor.  It might have something to do with the linking, so maybe
    // if StrategyEvaluator and OrderFunction are compiled in the same library
    // then it might work.
    //
    // Initialize the qsolve module
    //
    initQSolveModule();

    //
    // Create a test Cassandra Database
    //
    CassandraDatabase database( interpreter, "PyDatabase" );
    database.getData();

    //
    // Create a test Order Factory
    //
    OrderFactory order_factory( database );
    order_t order = order_factory.createOrder( time( NULL ), "AAPL", 1000 );
    cout << order.stock.symbol << endl;

    //
    // Create a test Strategy Evaluator
    const std::string strategyModule = "PyStrategy";
    interpreter.loadModule( strategyModule );
    PythonInterpreter::py_ptr&& strategy_fn = interpreter.loadFunction( "strategy", strategyModule );
    StrategyEvaluator strategy( strategy_fn );
    strategy.run( time( NULL ) );

    return 0;
  } catch( const exception& e ) {
    //
    // Most likely store() threw an exception meaning that an option
    // was not recognized, or one of the required options was not given.
    //
    cout << e.what() << endl;
    cout << endl;
    cout << visible << endl;
    return 0;
  }
}


