#include "MockDatabase.hpp"

MockDatabase::MockDatabase() {
  using namespace std;
  using namespace boost::gregorian;

  const string error( "Unable to open input file: table.csv" );

  ifstream ifs( "table.csv", ifstream::in );
  if(! ifs.good() ) {
    throw invalid_argument( error.c_str() );
  }

  string line;
  if(! ( getline( ifs, line ) ) )  { // throw away first line;
    throw invalid_argument( error.c_str() );
  }
  
  //
  // for this mock I just have data for AAPL
  //
  stock_map_t AAPL_map;
  const string symbol("AAPL");

  while( ifs >> line ) {
    string value;
    std::string line_error( "Malformed csv file: table.csv: " );
    
    auto found = line.find( "," );
    if( found == string::npos ) {
      line_error += line;
      throw invalid_argument( line_error.c_str() );
    }
    
    value = line.substr( 0, found);
    datetime date;
    try {
      //
      // this can throw std::invalid_argument if the date is not correct
      //
      datetime datetry( from_string( value ) );
      date = datetry;
    } catch( ... ) {
      cerr << "Date not valid " << value << endl;
      //
      // skip this data point, and try to do the rest
      //
      continue;
    }

    line.erase( 0, found + 1);
    found = line.find( "," );
    if( found == string::npos ) {
      line_error += line;
      throw invalid_argument( line_error.c_str() );
    }
    //
    // can throw std::invalid_argument or std::out_of_range
    //
    value = line.substr( 0, found );
    currency open = stod( value );
    
    line.erase( 0, found + 1 );
    found = line.find( "," );
    if( found == string::npos ) {
      line_error += line;
      throw invalid_argument( line_error.c_str() );
    }
    value = line.substr( 0, found );
    currency high = stod( value );
    
    line.erase( 0, found + 1 );
    found = line.find(",");
    if( found == string::npos ) {
      line_error += line;
      throw invalid_argument( line_error.c_str() );
    }
    value = line.substr( 0, found );
    currency low = stod( value );
    
    line.erase( 0, found + 1 );
    found = line.find( "," );
    if( found == string::npos ) {
      line_error += line;
      throw invalid_argument( line_error.c_str() );
    }
    value = line.substr( 0, found );
    currency close = stod( value );
    
    line.erase( 0, found + 1 );
    found = line.find( "," );
    if( found == string::npos ) {
      line_error += line;
      throw invalid_argument( line_error.c_str() );
    }
    value = line.substr( 0, found );
    shares volume = stoll( value );
    
    //
    // N.B. I am ignoring Adj Close
    // I am setting "openVolume" to be the same as "volume"
    // because Yahoo! didn't give me Open Volume data
    //
    
    //
    // TODO: get multiple symbols
    //
    stock_t * stock = new stock_t( date, symbol, open, close, volume, volume );
    AAPL_map.emplace( to_iso_extended_string( date ), *stock );
    delete stock;
  }
  
  database_.emplace( symbol, AAPL_map );
}

MockDatabase::~MockDatabase() {
}

stock_t MockDatabase::getData( const datetime& date, const std::string& symbol ) const {
  const auto& found = database_.find( symbol );
  if( found == database_.end() ) {
    stock_t emptyStock = { date, symbol, 0, 0, 0, 0 };
    return std::move( emptyStock );
  }

  const stock_map_t& stock_map = found->second;
  const std::string date_sz = to_iso_extended_string( date );
  const auto& found_date = stock_map.find( date_sz );

  if( found_date == stock_map.end() ) {
    stock_t emptyStock = { date, symbol, 0, 0, 0, 0 };
    return std::move( emptyStock );
  }

  // std::cout << "Returning data for " << symbol 
  // << " on: " << boost::gregorian::to_iso_extended_string( date ) 
  // << ": " << found_date->second.open << ", " 
  // << found_date->second.close << ", "
  // << found_date->second.volume << ", "
  // << found_date->second.openVolume
  // << std::endl;
  return found_date->second;
}
