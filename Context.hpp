#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <unordered_map>
#include <string>

#include "QTypes.hpp"
#include "Database.hpp"
#include "OrderFactory.hpp"

struct Context {
  Context();
  ~Context();

  void order( const char * symbol, long amount );
  long operator[]( const char * symbol );

  Context& setDatabase( Database * database );
  Context& setOrderFactory( OrderFactory * orderFactory );
  Context& setDate( const datetime& date );


  //
  // balance_ has to be public so that it can be
  // exposed as an attribute to the Python
  // Context class
  //
  long balance_;

  //
  // Current Date
  //
  datetime date_;

private:
  Database * database_;
  OrderFactory * orderFactory_;
  std::unordered_map<std::string, long> portfolio_;
};

#endif
