#include "Portfolio.hpp"

Portfolio::Portfolio( currency initialBalance, const datetime& startDate, const datetime& endDate ) : 
  balance_( initialBalance ),
  initialBalance_( initialBalance ),
  startDate_( startDate ),
  endDate_( endDate )
{
}

Portfolio::Portfolio( const Portfolio& portfolio ) :
  balance_( portfolio.balance_ ),
  holdings_( portfolio.holdings_ )
{
}

Portfolio::~Portfolio() {
}

void Portfolio::connectToInputSource( boost::signals2::connection connection ) {
  inputSource_.disconnect();
  inputSource_ = connection;
}

void Portfolio::addOrder( const order_t& aOrder ) {
  const std::string symbol = aOrder.stock.symbol;
  const currency value = aOrder.stock.close;
  const shares numOfShares = aOrder.numberPurchased;

  //
  // Push the order onto the queue mapped to that symbol
  // 
  holdings_[symbol.c_str()].push_back( aOrder );
  orderQ_.push( aOrder );

  //
  // Update the current balance:
  // If numOfShares > 0, then stock was bought so balance should decrease
  // If numOfShares < 0, then stock was sold so balance should increase
  //
  balance_ -= value * numOfShares;
}

void Portfolio::print( std::ofstream& os ) {
  if( balance_ >= 0 ) {
    os << "Balance: $" << balance_ << std::endl;
  } else {
    os << "Balance: ($" << -1 * balance_ << ")" << std::endl;
  }

  if(!holdings_.empty()) {
    os << "Holdings:\nsymbol\tshares\n---------------" << std::endl;
    
    for( auto& it : holdings_ ) {
      //
      // go through the order queue for that symbol
      // and add up how many shares have been bought
      // and sold to get the current number owned
      //
      shares numShares = 0;
      for( auto& symbol : it.second ) {
	if( symbol.stock.open != 0 ) {
	  numShares += symbol.numberPurchased;
	} else {
	  //
	  // handle order where we didn't have stock information
	  //
	  continue;
	}
      }
      if( numShares != 0 ) {
	os << it.first << "\t" << numShares << std::endl;
      }
    }
  }
  os << std::endl;
}

//
// TODO: maybe take a database as an argument so that
// I can look up the latest price, but then what
// do I do when I don't have data for a stock?
//
void Portfolio::printHistory( std::ofstream& os ) {
  using namespace std;
  using namespace boost::gregorian;

  //
  // Print csv column headers
  //
  os << "Date,Portfolio Value" << endl;

  currency currentBalance = initialBalance_;
  unordered_map<std::string, shares> currentPortfolio;
  unordered_map<std::string, currency> latestPrice;

  for( datetime curr = startDate_; curr <= endDate_; curr += boost::gregorian::days( 1 ) ) {
    while(! orderQ_.empty() ) {
      order_t order = orderQ_.front();
      if( order.stock.date > curr ) {
	//
	// done processing orders for this day
	//
	break;
      } else {
	//
	// remove order from queue
	//
	orderQ_.pop();
      }
      
      //
      // This will automatically account for 
      currentBalance -= order.stock.open * order.numberPurchased;
      if( order.stock.open != 0 ) {
	currentPortfolio[order.stock.symbol] += order.numberPurchased;
	latestPrice[order.stock.symbol] = order.stock.open;
      }
    }

    currency totalValue = 0;
    for( auto& it : currentPortfolio ) {
      totalValue += it.second * latestPrice[it.first];
    }

    os << to_iso_extended_string( curr ) << ",";
    os << totalValue + currentBalance << endl;
  }
}
