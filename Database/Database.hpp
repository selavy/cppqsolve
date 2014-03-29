#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "PythonInterpreter.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include "boost/noncopyable.hpp"

//
// Possible Cassandra C++ driver
// Check out: https://github.com/datastax/cpp-driver
//

class Database :
  boost::noncopyable
{
public:
  explicit Database( PythonInterpreter& interpreter, std::string PyModuleName, std::string PyFuncName = "getData" );
  virtual ~Database();
  void get();

private:
  PythonInterpreter::py_ptr PyFunc_;
};

#endif
