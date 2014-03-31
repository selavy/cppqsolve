#ifndef _SLIPPAGE_FUNCTION_HPP_
#define _SLIPPAGE_FUNCTION_HPP_

#include "PythonInterpreter.hpp"
#include "QTypes.hpp"
#include <utility> // std::move()

class SlippageFunction {
public:
  SlippageFunction( const PythonInterpreter::py_ptr& slippage );
  ~SlippageFunction();
  order_t operator()( const order_t& order );

private:
  PythonInterpreter::py_ptr& slippage_;
};

#endif
