#include "Database.hpp"

Database::Database( std::string PyModuleName, std::string PyFuncName ) {
  using namespace std;
  cout << "Creating database..." << endl;

  std::unique_ptr<PyObject, PyObjectDeleter> pModule;
  std::unique_ptr<PyObject, PyObjectDeleter> pName;
  std::unique_ptr<PyObject, PyObjectDeleter> pDict;

  Py_Initialize();

  pName.reset( PyString_FromString(PyModuleName.c_str()) );
  pModule.reset( PyImport_Import( pName.get() ) );
  pDict.reset( PyModule_GetDict( pModule.get() ) );
  PyFunc_.reset( PyDict_GetItemString( pDict.get(), PyFuncName.c_str() ) );

  if( !PyCallable_Check( PyFunc_.get() ) ) {
    throw std::invalid_argument( "Unable to open python database function" );
  }
}

Database::~Database() {
  Py_Finalize();
}

void Database::get() {
  PyObject_CallObject( PyFunc_.get(), NULL );
}
