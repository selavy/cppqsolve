#ifndef _QTYPES_HPP_
#define _QTYPES_HPP_

#include <ctime>
#include <string>

typedef long long shares;
typedef double currency;
typedef time_t date;

struct stock_date_t {
  std::string symbol;
  currency open;
  currency close;
  shares volume;
  shares openVolume;
};

struct order_t {
  stock_date_t stock;
  date purchaseDate;
  shares numberPurchased;
  shares symbolVolume;
};

#endif
