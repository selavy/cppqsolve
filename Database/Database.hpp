#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <Python.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "boost/noncopyable.hpp"

//
// Possible Cassandra C++ driver
// Check out: https://github.com/datastax/cpp-driver
//

struct PyObjectDeleter {
  template <typename T>
  void operator()(T* p) {
    Py_DECREF(p);
    p = NULL;
  }
};

class Database :
  boost::noncopyable
{
public:
  explicit Database( std::string PyModuleName, std::string PyFuncName = "getData" );
  virtual ~Database();

  void get();

private:
  std::unique_ptr<PyObject, PyObjectDeleter> PyFunc_;
};

#endif
