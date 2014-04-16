#ifndef _ORDER_FACTORY_HPP_
#define _ORDER_FACTORY_HPP_

#include "Database.hpp"
#include "QTypes.hpp"

class OrderFactory {
public:
  explicit OrderFactory( const Database& database );
  virtual ~OrderFactory();
  order_t createOrder( const datetime& date, const std::string& symbol, shares numPurchased );

private:
  const Database& database_;
};

#endif
