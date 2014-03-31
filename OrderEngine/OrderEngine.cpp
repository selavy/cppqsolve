#include "OrderEngine.hpp"

OrderEngine::OrderEngine( OrderFactory& orderFactory ) :
  orderFactory_( orderFactory )
{
  using namespace std;
  cout << "Creating an OrderEngine..." << endl;
}

OrderEngine::~OrderEngine() {
  //
  // TODO: Is this unnecessary?
  //
  inputConnection_.disconnect();
}

void OrderEngine::connectToOrderInputSource( boost::signals2::connection connection ) {
  inputConnection_.disconnect();
  inputConnection_ = connection;
}

void OrderEngine::handleOrder( const datetime& date, const std::string& symbol, long numOfShares ) {
  order_t order = orderFactory_.createOrder( date, symbol, numOfShares );

  //
  // TODO: call slippage function and commission function
  //
  orderQueue_.push( order );
}

std::ostream& OrderEngine::printOrderQueue( std::ostream& os ) {
  while( !orderQueue_.empty() ) {
    order_t order = orderQueue_.front();
    orderQueue_.pop();
    os << order.stock.symbol << " " << order.numberPurchased << std::endl;
  }
  return os;
}

