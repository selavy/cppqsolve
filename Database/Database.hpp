#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "boost/noncopyable.hpp"
#include "QTypes.hpp"

//
// Interface for the Database class
//
class Database :
  boost::noncopyable
{
public:
  virtual ~Database() {}
  virtual stock_t getData() const = 0;
};

#endif
