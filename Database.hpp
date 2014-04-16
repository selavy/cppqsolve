#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "boost/noncopyable.hpp"
#include "QTypes.hpp"
#include <string>

//
// Interface for the Database class
//
class Database :
  boost::noncopyable
{
public:
  virtual ~Database() {}
  virtual stock_t getData( const datetime& date, const std::string& symbol ) const = 0;
};

#endif
