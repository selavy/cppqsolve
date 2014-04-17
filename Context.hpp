#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <memory>
#include <unordered_map>
#include <string>

#include "QTypes.hpp"
#include "OrderEngine.hpp"
#include "Portfolio.hpp"

struct Context {
  Context();
  ~Context();

  void order( const char * symbol, shares amount );
  long operator[]( const char * symbol );

  Context& setOrderEngine( OrderEngine * orderEngine );
  Context& setDate( const datetime& date );
  Context& setStartDate( const datetime& date, bool setDate = false );
  Context& setEndDate( const datetime& date );
  void printTransactionList( std::ofstream& os ) const;
  void printHistory( std::ofstream& os ) const;
  void processOrderQueue( const datetime& date );

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
  OrderEngine * orderEngine_;
  std::shared_ptr<Portfolio> portfolio_;
};

#endif
