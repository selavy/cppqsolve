#ifndef _PYTHON_INTERPRETER_HPP_
#define _PYTHON_INTERPRETER_HPP_

#include <Python.h>
#include <iostream>
#include "boost/noncopyable.hpp"
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

//
// N.B.: Only make 1 instance of this class
//
class PythonInterpreter :
  boost::noncopyable
{
public:
  //
  // Policy class for smart pointers to delete PyObjects*
  //
  struct PyObjectDeleter
  {
    template <typename T>
    void operator()(T* p) {
      Py_DECREF(p);
      p = NULL;
    }
  };
  
  typedef std::unique_ptr<PyObject, PythonInterpreter::PyObjectDeleter> py_ptr;

public:
  PythonInterpreter( int argc, char **argv );
  virtual ~PythonInterpreter();
  void loadModule( const std::string& moduleName );
  PythonInterpreter::py_ptr loadFunction( const std::string& functionName, const std::string& moduleName );

private:
  std::unordered_map<std::string, PyObject*> modulesList_;
};

#endif
