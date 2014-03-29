#include "Portfolio.hpp"

Portfolio::Portfolio() : 
  balance_( 0 )
{
}

Portfolio::Portfolio( const Portfolio& portfolio ) :
  balance_( portfolio.balance_ ),
  holdings_( portfolio.holdings_ )
{
}

Portfolio::~Portfolio() {
}

void Portfolio::addOrder( const order_t& aOrder ) {
  const std::string symbol = aOrder.stock.symbol;
  const currency value = aOrder.stock.close;
  const shares numOfShares = aOrder.numberPurchased;

  //
  // Push the order onto the queue mapped to that symbol
  // 
  holdings_[symbol.c_str()].push_back( aOrder );

  //
  // Update the current balance:
  // If numOfShares > 0, then stock was bought so balance should decrease
  // If numOfShares < 0, then stock was sold so balance should increase
  //
  balance_ -= value * numOfShares;
}

void Portfolio::print( std::ostream& os ) {
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
	numShares += symbol.numberPurchased;
      }
      if( numShares != 0 ) {
	os << it.first << "\t" << numShares << std::endl;
      }
    }
  }
  os << std::endl;
}
