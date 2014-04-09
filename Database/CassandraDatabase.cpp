#include "CassandraDatabase.hpp"

CassandraDatabase::CassandraDatabase( PythonInterpreter& interpreter, std::string PyModuleName, std::string PyFuncName ) {
  //
  // Both of these functions can throw std::invalid_argument
  //
  interpreter.loadModule( PyModuleName );
  PyFunc_ = interpreter.loadFunction( PyFuncName, PyModuleName );
}

CassandraDatabase::~CassandraDatabase() {
}

stock_t CassandraDatabase::getData( const datetime& date ) const {
  PyObject_CallObject( PyFunc_.get(), NULL );
  stock_t retVal = { date, "AAPL", 537.71, 537.46, 11495400, 7163009 };
  return std::move( retVal );
}
