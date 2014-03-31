#ifndef _ORDER_ENGINE_HPP_
#define _ORDER_ENGINE_HPP_

#include <iostream>
#include <queue>
#include <mutex>
#include <stdexcept>
#include "boost/signals2/signal.hpp"
#include "boost/signals2/connection.hpp"
#include "boost/noncopyable.hpp"
#include "QTypes.hpp"
#include "OrderFactory.hpp"

class OrderEngine :
  boost::noncopyable
{
public:
  typedef boost::signals2::signal<void (const order_t&)> signal_t;

public:
  //
  // TODO: add connection to SlippageFunction and Commission Function
  //
  explicit OrderEngine( OrderFactory& orderFactory );
  ~OrderEngine();

  void connectToOrderInputSource( boost::signals2::connection connection );
  void handleOrder( const datetime& date, const std::string& companyName, long numOfShares );
  boost::signals2::connection connectToPortfolio( const signal_t::slot_type& subscriber );
  std::ostream& printOrderQueue( std::ostream& os );
  void processOrderQueue( const datetime& date );

private:
  OrderFactory& orderFactory_;
  boost::signals2::connection inputConnection_;
  std::mutex queueLock_;
  std::queue<order_t> orderQueue_;
  std::queue<order_t> filledOrderQueue_;
  signal_t filledOrderSig_;
};

#endif
