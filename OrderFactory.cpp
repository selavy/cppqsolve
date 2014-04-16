#include "OrderFactory.hpp"

OrderFactory::OrderFactory( const Database& database ) : database_( database ) {
}

OrderFactory::~OrderFactory() {
}

order_t OrderFactory::createOrder( const datetime& date, const std::string& symbol, shares numPurchased ) {
  stock_t stock = database_.getData( date, symbol );
  order_t order = { stock, numPurchased };
  return std::move( order );
}
