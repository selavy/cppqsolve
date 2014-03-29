#include "Database.hpp"

Database::Database( PythonInterpreter& interpreter, std::string PyModuleName, std::string PyFuncName ) {
  //
  // Both of these functions can throw std::invalid_argument
  //
  interpreter.loadModule( PyModuleName );
  PyFunc_ = interpreter.loadFunction( PyFuncName, PyModuleName );
}

Database::~Database() {
}

void Database::get() {
  PyObject_CallObject( PyFunc_.get(), NULL );
}
