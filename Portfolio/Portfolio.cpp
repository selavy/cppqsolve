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
  orderQ_.push_back( aOrder );

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

void Portfolio::printTransactionList( std::ofstream& os ) const {
  using namespace std;
  using namespace boost::gregorian;
  
  os << "Date,Symbol,Action,#Shares,Price,$Amount" << endl;

  for( const auto& it : orderQ_ ) {
    if( it.numberPurchased == 0 ) {
      continue;
    }

    os << to_iso_extended_string( it.stock.date ) << ",";
    os << it.stock.symbol << ",";
    os << ((it.numberPurchased > 0) ? "BUY" : "SELL") << ",";
    os << it.numberPurchased << ",";
    os << it.stock.open << ",";
    os << it.stock.open * it.numberPurchased;
    os << endl;
  }
}

//
// TODO: maybe take a database as an argument so that
// I can look up the latest price, but then what
// do I do when I don't have data for a stock?
//
void Portfolio::printHistory( std::ofstream& os ) const {
  using namespace std;
  using namespace boost::gregorian;

  //
  // Print csv column headers
  //
  os << "Date,Portfolio Value,Returns" << endl;

  currency currentBalance = initialBalance_;
  currency totalSpent = 0;
  currency totalMade = 0;
  unordered_map<std::string, shares> currentPortfolio;
  unordered_map<std::string, currency> latestPrice;
  auto orderIt = orderQ_.begin();

  for( datetime curr = startDate_; curr <= endDate_; curr += boost::gregorian::days( 1 ) ) {
    while( orderIt != orderQ_.end() ) {
      order_t order = *orderIt;
      if( order.stock.date > curr ) {
	//
	// done processing orders for this day
	//
	break;
      } else {
	//
	// remove order from queue
	//
	++orderIt;
      }
      
      //
      // This will automatically account for 
      currentBalance -= order.stock.open * order.numberPurchased;
      if( order.numberPurchased > 0 ) {
	//
	// if bought, then add to Total Amount Spent
	//
	totalSpent += order.stock.open * order.numberPurchased;
      } else {
	//
	// order.numberPurchased will be negative, so subtract to avoid
	// multiplying by -1
	//
	totalMade -= order.stock.open * order.numberPurchased;
      }
      if( order.stock.open != 0 ) {
	currentPortfolio[order.stock.symbol] += order.numberPurchased;
	latestPrice[order.stock.symbol] = order.stock.open;
      }
    }

    currency totalValue = 0;
    for( auto& it : currentPortfolio ) {
      totalValue += it.second * latestPrice[it.first];
    }

#define DEBUG
#ifdef DEBUG
    os << "DEBUG: ";
    for( auto& it : currentPortfolio ) {
      os << it.first << " : " << it.second << " @ " << latestPrice[it.first] << endl;
    }
    os << "Balance: " << currentBalance << endl;
    os << "Total Value: " << totalValue << endl;
    os << "Total Spent: " << totalSpent << endl;
    os << "Total Made: " << totalMade << endl;
#endif
    os << to_iso_extended_string( curr ) << ",";
    os << totalValue + currentBalance << ",";
    os << (totalSpent == 0) ? 0.000 : (static_cast<double>(totalMade - totalSpent) / static_cast<double>(totalSpent));
    os << endl;

#ifdef DEBUG
    os << endl;
#endif
  }


  os << "FINAL: " << endl;
  for( auto& it : currentPortfolio ) {
    os << it.first << " : " << it.second << " @ " << latestPrice[it.first] << endl;
  }
}
