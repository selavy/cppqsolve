#ifndef _ORDER_ENGINE_HPP_
#define _ORDER_ENGINE_HPP_

#include <iostream>
#include <queue>
#include "boost/signals2/connection.hpp"
#include "boost/noncopyable.hpp"
#include "QTypes.hpp"
#include "OrderFactory.hpp"

class OrderEngine :
  boost::noncopyable
{
public:
  explicit OrderEngine( OrderFactory& orderFactory );
  ~OrderEngine();

  void connectToOrderInputSource( boost::signals2::connection connection );
  void handleOrder( const datetime& date, const std::string& companyName, long numOfShares );

  std::ostream& printOrderQueue( std::ostream& os );

private:
  boost::signals2::connection inputConnection_;
  std::queue<order_t> orderQueue_;
  OrderFactory& orderFactory_;
};

#endif
