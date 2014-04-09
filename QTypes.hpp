#ifndef _QTYPES_HPP_
#define _QTYPES_HPP_

#include <ctime>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp" // does not include I/O

typedef long long shares;
typedef double currency;
//typedef time_t datetime;
typedef boost::gregorian::date datetime;

struct stock_t {
  datetime date;
  std::string symbol;
  currency open;
  currency close;
  shares volume;
  shares openVolume;
};

struct order_t {
  stock_t stock;
  shares numberPurchased;
};

#endif
