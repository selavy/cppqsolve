#include "OrderFactory.hpp"

OrderFactory::OrderFactory( const Database& database ) : database_( database ) {
}

OrderFactory::~OrderFactory() {
}

order_t OrderFactory::createOrder( datetime date, const std::string& symbol, shares numPurchased ) {
  stock_t stock = database_.getData();
  order_t order = { stock, numPurchased };
  return std::move( order );
}
