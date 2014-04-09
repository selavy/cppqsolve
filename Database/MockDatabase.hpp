#ifndef _MOCKDATABASE_HPP_
#define _MOCKDATABASE_HPP_

#include <string>
#include <fstream>
#include "Database.hpp"
#include <unordered_map>
#include <stdexcept>
#include <iostream>

class MockDatabase :
  public Database
{
public:
  typedef std::unordered_map<std::string, stock_t> stock_map_t;
  typedef std::unordered_map<std::string, stock_map_t> mdatabase_t;

public:
  MockDatabase();
  virtual ~MockDatabase();
  virtual stock_t getData( const datetime& date, const std::string& symbol ) const;

private:
  mdatabase_t database_;
};  

#endif
