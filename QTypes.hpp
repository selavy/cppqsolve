#ifndef _QTYPES_HPP_
#define _QTYPES_HPP_

#include <ctime>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp" // does not include I/O

typedef long long shares;
typedef double currency;
typedef boost::gregorian::date datetime;

struct stock_t {
  datetime date;
  std::string symbol;
  currency open;
  currency close;
  shares volume;
  shares openVolume;

  stock_t( const datetime& date_, const std::string& symbol_, const currency& open_,
	   const currency& close_, const shares& volume_, const shares& openVolume_ ) :
    date( date_ ), symbol( symbol_ ), open( open_ ), close( close_ ), volume( volume_ ),
    openVolume( openVolume_ ) {}
};

struct order_t {
  stock_t stock;
  shares numberPurchased;
};

#endif
