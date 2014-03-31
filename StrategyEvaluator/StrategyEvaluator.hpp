#ifndef _STRATEGY_EVALUATOR_HPP_
#define _STRATEGY_EVALUATOR_HPP_

#include "PythonInterpreter.hpp" /* must come first or will see a bunch of warnings from Python.h */
#include <stdexcept>
#include <string>
#include <queue>
#include "boost/noncopyable.hpp"
#include "QTypes.hpp"

class StrategyEvaluator :
  boost::noncopyable
{
public:
  StrategyEvaluator( PythonInterpreter::py_ptr& strategy );
  virtual ~StrategyEvaluator();
  void run( datetime date );
  void addOrder( const std::string& symbol, long numOfShares );

private:
  PythonInterpreter::py_ptr& strategy_;
  std::queue<order_t> orderQueue_;
};

#endif
