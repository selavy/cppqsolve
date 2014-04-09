#include "OrderEngine.hpp"

OrderEngine::OrderEngine( OrderFactory& orderFactory ) :
  orderFactory_( orderFactory )
{
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
  using namespace std;
  order_t order = orderFactory_.createOrder( date, symbol, numOfShares );
  /*
  if( order.stock.open == 0 ) {
    //
    // Attempted to buy on a time point for which we don't have data
    //
    //cerr << "Dropped order for " << numOfShares << " of " << symbol << " on "
    //	 << boost::gregorian::to_iso_extended_string( date ) << endl;
    return;
  }
  */

  try {
    queueLock_.lock();
    orderQueue_.push( order );
    queueLock_.unlock();
  } catch( ... ) {
    //
    // make sure to unlock mutex otherwise might get deadlock
    //
    queueLock_.unlock();
    throw std::overflow_error( "dropped order because could not push it on the order queue" );
  }
}

boost::signals2::connection OrderEngine::connectToPortfolio( const signal_t::slot_type& subscriber ) {
  return filledOrderSig_.connect( subscriber );
}

//
// don't use this method, just for testing purposes
//
std::ostream& OrderEngine::printOrderQueue( std::ostream& os ) {
  queueLock_.lock();
  while( !orderQueue_.empty() ) {
    order_t order = orderQueue_.front();
    orderQueue_.pop();
    os << order.stock.symbol << " " << order.numberPurchased << std::endl;
  }
  return os;
}

void OrderEngine::processOrderQueue( const datetime& date ) {
  //
  // TODO: Use slippage and commission functions
  //
  try {
    //
    // get mutex for orderQueue
    //
    queueLock_.lock();
    
    while( !orderQueue_.empty() ) {
      order_t order = orderQueue_.front();
      if( order.stock.date > date ) {
	//
	// purchase is for in the future
	// so leave it on the queue for next time.
	//
	break;
      } else {
	//
	// remove order from queue
	//
	orderQueue_.pop();
	
	//
	// TODO: check if meet requirements and adjust price using slippage function 
	//

	//
	// TODO: adjust price based on commission function
	//

	filledOrderQueue_.push( order );
	filledOrderSig_( order );
      }
    }

    //
    // release mutex for orderQueue
    //
    queueLock_.unlock();
  } catch( ... ) {
    //
    // make sure to release mutex for orderQueue or may have deadlock
    //
    queueLock_.unlock();
  }
}
