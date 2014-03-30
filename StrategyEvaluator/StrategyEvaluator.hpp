#ifndef _STRATEGY_EVALUATOR_HPP_
#define _STRATEGY_EVALUATOR_HPP_

#include "PythonInterpreter.hpp"
#include "QTypes.hpp"
#include "Database.hpp"
#include <stdexcept>
#include <string>
#include <queue>

class StrategyEvaluator {
public:
  StrategyEvaluator( Database& database, PythonInterpreter::py_ptr& strategy );
  virtual ~StrategyEvaluator();
  void run( datetime date );

private:
  Database& database_;
  PythonInterpreter::py_ptr& strategy_;
  std::queue<order_t> orderQueue_;
};

#endif
