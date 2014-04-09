#ifndef _STRATEGY_EVALUATOR_HPP_
#define _STRATEGY_EVALUATOR_HPP_

#include "PythonInterpreter.hpp" /* must come first or will see a bunch of warnings from Python.h */
#include <stdexcept>
#include <string>
#include "boost/noncopyable.hpp"
#include <boost/signals2/signal.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/bind.hpp>
#include "QTypes.hpp"

class StrategyEvaluator :
  boost::noncopyable
{
public:
  typedef boost::signals2::signal<void (const datetime&, const std::string&, long)> signal_t;
public:
  StrategyEvaluator( PythonInterpreter::py_ptr& strategy );
  virtual ~StrategyEvaluator();
  boost::signals2::connection connectToOrderHandler( const signal_t::slot_type& subscriber );
  void run( const datetime& date );
  void addOrder( const std::string& symbol, long numOfShares );

private:
  PythonInterpreter::py_ptr& strategy_;
  signal_t orderSig_;
  datetime currentDate_;
};

#endif
