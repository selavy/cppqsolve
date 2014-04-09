#ifndef _PORTFOLIO_HPP_
#define _PORTFOLIO_HPP_

#include "QTypes.hpp"
#include "boost/signals2/connection.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

class Portfolio {
public:
  explicit Portfolio( currency initialBalance = 0, const datetime& startDate = boost::gregorian::day_clock::local_day(), const datetime& endDate = boost::gregorian::day_clock::local_day() );
  Portfolio( const Portfolio& portfolio );
  virtual ~Portfolio();

  void connectToInputSource( boost::signals2::connection connection );
  void addOrder( const order_t& aOrder );
  void print( std::ofstream& os );
  void printTransactionList( std::ofstream& os ) const;
  void printHistory( std::ofstream& os ) const;

private:
  currency balance_;
  currency initialBalance_;
  datetime startDate_;
  datetime endDate_;
  std::unordered_map<std::string, std::vector<order_t>> holdings_;
  std::vector<order_t> orderQ_;
  boost::signals2::connection inputSource_;
};

#endif
