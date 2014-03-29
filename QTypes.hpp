#ifndef _QTYPES_HPP_
#define _QTYPES_HPP_

#include <ctime>
#include <string>

typedef long long shares;
typedef double currency;
typedef time_t datetime;

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
