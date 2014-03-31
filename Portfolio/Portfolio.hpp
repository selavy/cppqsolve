#ifndef _PORTFOLIO_HPP_
#define _PORTFOLIO_HPP_

#include "QTypes.hpp"
#include "boost/signals2/connection.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

class Portfolio {
public:
  Portfolio();
  Portfolio( const Portfolio& portfolio );
  virtual ~Portfolio();

  void connectToInputSource( boost::signals2::connection connection );
  void addOrder( const order_t& aOrder );
  void print( std::ostream& os );

private:
  currency balance_;
  std::unordered_map<std::string, std::vector<order_t>> holdings_;
  boost::signals2::connection _inputSource;
};

#endif
