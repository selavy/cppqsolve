#ifndef _CASSANDRA_DATABASE_HPP_
#define _CASSANDRA_DATABASE_HPP_

#include "PythonInterpreter.hpp"
#include "Database.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include "boost/noncopyable.hpp"

//
// Possible Cassandra C++ driver
// Check out: https://github.com/datastax/cpp-driver
//

class CassandraDatabase :
  public Database
{
public:
  explicit CassandraDatabase( PythonInterpreter& interpreter, std::string PyModuleName, std::string PyFuncName = "getData" );
  virtual ~CassandraDatabase();
  virtual stock_t getData() const;

private:
  PythonInterpreter::py_ptr PyFunc_;
};

#endif
