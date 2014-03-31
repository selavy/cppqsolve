#include "SlippageFunction.hpp"

SlippageFunction::SlippageFunction( const PythonInterpreter::py_ptr& slippage )
  : slippage_( slippage )
{
}

SlippageFunction::~SlippageFunction() {
}

order_t SlippageFunction::operator()( const order_t& order ) {
  order_t new_order;
  return std::move( new_order );
}
