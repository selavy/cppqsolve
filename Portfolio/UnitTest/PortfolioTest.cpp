#include <iostream>
#include "Portfolio.hpp"

int main(int argc, char **argv) {
  Portfolio portfolio;
  time_t currentTime = time( NULL );
  stock_t stock1 = { currentTime, "AAPL", 537.71, 537.46, 9349000, 7163009 }; 
  stock_t stock2 = { currentTime + 2, "GE", 25.90, 25.81, 37138300, 29691350 };
  order_t order1 = { stock1, 200 };
  order_t order2 = { stock2, 500 };
  order_t order3 = { stock1, -200 };
  portfolio.print( cout );
  portfolio.addOrder( order1 );
  portfolio.addOrder( order1 );
  portfolio.print( cout );
  portfolio.addOrder( order2 );
  portfolio.print( cout );
  portfolio.addOrder( order3 );
  portfolio.print( cout );

  return 0;
}
