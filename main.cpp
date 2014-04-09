#include "PythonInterpreter.hpp" // get rid of warning about Python.h not being first include
#include <iostream>
#include <fstream>
#include <string>
#include "QTypes.hpp"
#include "Config.h"
//#include "CassandraDatabase.hpp"
#include "MockDatabase.hpp"
#include "OrderFactory.hpp"
#include "OrderEngine.hpp"
#include "OrderFunction.hpp"
#include "Portfolio.hpp"
#include "StrategyEvaluator.hpp"
#include "boost/program_options.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace std;

int main( int argc, char **argv ) {
  //
  // Use Boost.Program_Options to parse the command line and configuration file input.
  // Documentation for this library available at: http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html
  //

  string config_file;
  string strategyFile;
  currency initialBalance;
  string startDate_sz;
  string endDate_sz;
  datetime date;
  datetime endDate;
  //ostream out;
  ofstream ofs;

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
    ( "start,s", boost::program_options::value<string>(&startDate_sz)->required(), "start date for simulation" )
    ( "end,e", boost::program_options::value<string>(&endDate_sz), "end date for simulation" )
    ( "balance,b", boost::program_options::value<currency>(&initialBalance)->default_value(0), "Initial amount of currency available" )
    ( "output,o", boost::program_options::value<string>(), "Output file location, if one already exists, it will be overwritten" )
    ;

  //
  // Hidden options can be specified on the command line or in the configuration file, but
  // they will not be displayed in the help message because they are positional variables.
  //
  boost::program_options::options_description hidden( "Hidden Options" );
  hidden.add_options()
    ( "strategy", boost::program_options::value<string>(&strategyFile)->required(), "python strategy file" )
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
#ifdef DEBUG
      cerr << visible << endl;
#endif
      return 0;
    }
    
    if( vm.count( "version" ) ) {
      print_version( argv[0] );
      return 0;
    }
    
    if( vm.count( "start" ) ) {
      try {
	boost::gregorian::date startDateTmp( boost::gregorian::from_string( startDate_sz ) );
	date = startDateTmp;
      } catch( ... ) {
	std::string error = "Error: unable to parse start date: " + startDate_sz;
	throw std::invalid_argument( error.c_str() );
      }
#ifdef DEBUG
      cerr << "Start date set to "
	   << vm["start"].as<string>()
	   << endl;
#endif
    } else {
      throw std::invalid_argument("Must provide a start date for simulation.");
    }
    
    if( vm.count( "end" ) ) {
      try {
	boost::gregorian::date endDateTmp( boost::gregorian::from_string( endDate_sz ) );
	endDate = endDateTmp;
      } catch( ... ) {
	std::string error = "Error: unable to parse end date: " + endDate_sz;
	throw std::invalid_argument( error.c_str() );
      }
      
#ifdef DEBUG
      cerr << "End date set to "
	   << endDate_sz
	   << endl;
#endif
    } else {
      endDate = boost::gregorian::day_clock::local_day();
#ifdef DEBUG
      cerr << "No end date given.  Using current date." << endl;
#endif
    }

    if( vm.count( "output" ) ) {
      ofs.open( vm["output"].as<string>().c_str(), ofstream::out | ofstream::trunc );
      if(! ofs.good() ) {
	string error("Unable to open output file: ");
	error += vm["output"].as<string>();
	throw std::invalid_argument( error.c_str() );
      }
    } else {
      ofs.copyfmt( cout );
      ofs.clear( cout.rdstate() );
      ofs.basic_ios<char>::rdbuf( cout.rdbuf() );
    }
    
    if( vm.count( "strategy" ) ) {
#ifdef DEBUG
      cerr << "Strategy file set to "
	   << vm["strategy"].as<string>()
	   << endl;
#endif
    } else {
      cerr << "Must provide a strategy file." << endl;
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
    //CassandraDatabase database( interpreter, "PyDatabase" );
    MockDatabase database;

    //
    // Create a test Order Factory
    //
    OrderFactory orderFactory( database );

    //
    // Create a test Strategy Evaluator
    //
    std::string strategyModule = vm["strategy"].as<string>();
    
    //
    // Check if the python file given has the .py extension and remove it
    // if it does, otherwise do nothing
    //
    std::string key (".py");
    auto found = strategyModule.rfind( key );
    if( found != std::string::npos ) {
      strategyModule.erase( found, key.length() );
    }
    
    interpreter.loadModule( strategyModule );
    PythonInterpreter::py_ptr&& strategy_fn = interpreter.loadFunction( "strategy", strategyModule );
    StrategyEvaluator strategy( strategy_fn );

    //
    // Create an Order Engine
    //
    OrderEngine orderEngine( orderFactory );
    
    //
    // Create an Portfolio
    // TODO: Get initial balance
    //
    Portfolio portfolio( 0, date, endDate );

    //
    // Connect Strategy and Order Engine
    //
    orderEngine.connectToOrderInputSource( strategy.connectToOrderHandler( boost::bind( &OrderEngine::handleOrder, &orderEngine, _1, _2, _3 ) ) );
    portfolio.connectToInputSource( orderEngine.connectToPortfolio( boost::bind( &Portfolio::addOrder, &portfolio, _1 ) ) );
    while( date <= endDate ) {
      strategy.run( date );
      orderEngine.processOrderQueue( date );
#ifdef DEBUG
      ofs << boost::gregorian::to_iso_extended_string( date ) << endl;
      portfolio.print( ofs );
#endif
      date += boost::gregorian::days(1);
    }

    portfolio.printHistory( ofs );
    
    if( ofs.is_open() ) {
      ofs.close();
    }
    return 0;
  } catch( const exception& e ) {
    //
    // Most likely store() threw an exception meaning that an option
    // was not recognized, or one of the required options was not given.
    //
    if( ofs.is_open() ) {
      ofs.close();
    }
    cerr << e.what() << endl;
    cerr << endl;
    cerr << visible << endl;
    return 0;
  }
}


