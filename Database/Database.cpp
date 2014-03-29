#include "Database.hpp"

Database::Database( int argc, char ** argv, std::string PyModuleName, std::string PyFuncName ) {
  using namespace std;
  std::unique_ptr<PyObject, PyObjectDeleter> pModule;
  std::unique_ptr<PyObject, PyObjectDeleter> pName;

  Py_SetProgramName(argv[0]);
  Py_Initialize();
  PySys_SetArgv( argc, argv );

  pName.reset( PyString_FromString(PyModuleName.c_str()) );
  if( !pName ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::invalid_argument( "Unable to create PyString from string" );
  }

  pModule.reset( PyImport_Import( pName.get() ) );
  if( !pModule ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::invalid_argument( "Unable to find module" );
  }

  PyFunc_.reset( PyObject_GetAttrString( pModule.get(), PyFuncName.c_str() ) );
  if( !PyFunc_ || !PyCallable_Check( PyFunc_.get() ) ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::invalid_argument( "Unable to get function" );
  }
}

Database::~Database() {
  Py_Finalize();
}

void Database::get() {
  PyObject_CallObject( PyFunc_.get(), NULL );
}
