#ifndef _QTYPES_HPP_
#define _QTYPES_HPP_

#include <ctime>
#include <string>

typedef long long amount;
typedef double capital;
typedef time_t date;

struct order_t {
  std::string stockSymbol;
  date purchaseDate;
  amount numberPurchased;
  amount symbolVolume;
  capital pricePerShare;
};

#endif
