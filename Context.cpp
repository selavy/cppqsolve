#include "Context.hpp"

Context::Context() : balance_( 0 ), portfolio_( new Portfolio() ), orderEngine_( NULL ), database_( NULL ) {}

Context::~Context() {
}

//
// This function just serves to connect a particular
// Python strategy to a Context structure, it will
// send the order to the OrderFactory to determine
// what should be done with it
//
void Context::order( const char * symbol, shares amount ) {
  using namespace std;
  cout << "Received order for: " << symbol << " at " << amount << endl;

  if( orderEngine_ != NULL ) {
    std::string aSymbol( symbol );
    orderEngine_->handleOrder( date_, aSymbol, amount );
  }
}

long Context::operator[]( const char * symbol ) {
  return portfolio_->get( symbol );
}

Context& Context::setOrderEngine( OrderEngine * orderEngine ) {
  orderEngine_ = orderEngine;
  orderEngine_->connectToPortfolio( boost::bind( &Portfolio::addOrder, portfolio_.get(), _1 ) );
  return( *this );
}

Context& Context::setDatabase( Database * database ) {
  database_ = database;
  return( *this );
}

Context& Context::setDate( const datetime& date ) {
  date_ = date;
  return( *this );
}

Context& Context::setStartDate( const datetime& date, bool setDate ) {
  if( setDate ) {
    date_ = date;
  }
  portfolio_->setStartDate( date );
  return( *this );
}

Context& Context::setEndDate( const datetime& date ) {
  portfolio_->setEndDate( date );
  return( *this );
}

void Context::printTransactionList( std::ofstream& os ) const {
  portfolio_->printTransactionList( os );
}

void Context::printHistory( std::ofstream& os ) const {
  portfolio_->printHistory( os );
}

void Context::processOrderQueue( const datetime& date ) {
  if( orderEngine_ != NULL ) {
    orderEngine_->processOrderQueue( date );
  }
}

stock_t Context::getData( const datetime& date, const std::string& symbol ) const {
  if( database_ == NULL ) {
    throw std::runtime_error( "Error in Context->getData(): Database not set!" );
  }

  return database_->getData( date, symbol );
}
