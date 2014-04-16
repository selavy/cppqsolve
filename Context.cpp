#include "Context.hpp"

Context::Context() : balance_( 0 ), database_( NULL ), orderFactory_( NULL ) {}

Context::~Context() {}

//
// This function just serves to connect a particular
// Python strategy to a Context structure, it will
// send the order to the OrderFactory to determine
// what should be done with it
//
void Context::order( const char * symbol, long amount ) {
  using namespace std;
  cout << "Received order for: " << symbol << " at " << amount << endl;

  //
  // TODO: send order to the OrderFactory to be processed
  //
  if( database_ != NULL ) {
    cout << "Current Price: " << ( database_->getData( date_, symbol ) ).open << endl;
  } 
}

long Context::operator[]( const char * symbol ) {
  using namespace std;
  string key( symbol );
  return portfolio_[key];
}

Context& Context::setDatabase( Database * database ) {
  database_ = database;
  return( *this );
}

Context& Context::setOrderFactory( OrderFactory * orderFactory ) {
  orderFactory_ = orderFactory;
  return( *this );
}

Context& Context::setDate( const datetime& date ) {
  date_ = date;
}
