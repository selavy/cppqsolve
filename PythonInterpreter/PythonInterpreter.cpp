#include "PythonInterpreter.hpp"

PythonInterpreter::PythonInterpreter( int argc, char ** argv ) {
  if( !Py_IsInitialized() ) {
    Py_SetProgramName( argv[0] );
    Py_Initialize();
    PySys_SetArgv( argc, argv );
  } else {
    std::cerr << "WARNING: probably have two instances of PythonInterpreter!!!" << std::endl;
  }
}

PythonInterpreter::~PythonInterpreter() {
  //
  // Remove all of the PyObject* in the modulesList_
  //
  for( auto& it : modulesList_ ) {
    Py_DECREF( it.second );
  }

  //
  // Close the python interpreter
  //
  Py_Finalize();
}

void PythonInterpreter::loadModule( std::string moduleName ) {
  const auto got = modulesList_.find( moduleName );
  if( got != modulesList_.end() ) {
    //
    // Module has already been loaded
    //
    return;
  }
  
  PyObject * pyName = PyString_FromString( moduleName.c_str() );
  if( !pyName ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::invalid_argument( "Unable to create PyString from string" );
  }

  PyObject * pyModule = PyImport_Import( pyName );
  Py_DECREF( pyName );
  if( !pyModule ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::invalid_argument( "Unable to find module" );
  }
  
  modulesList_.emplace( moduleName, pyModule );
}

//
// N.B. return value uses move semantics
//
PythonInterpreter::py_ptr PythonInterpreter::loadFunction( std::string functionName, std::string moduleName ) {
  if( modulesList_.count( moduleName.c_str() ) == 0 ) {
    std::string errorString = "Error module not loaded: " + moduleName;
    throw std::invalid_argument( errorString );
  }

  PythonInterpreter::py_ptr pyFunc( PyObject_GetAttrString( modulesList_[moduleName.c_str()],
							   functionName.c_str()
							   ));
  if( !pyFunc ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    std::string errorString = "Error function " + functionName +
      " could not be located in " + moduleName;
    throw std::invalid_argument( errorString );
  }

  return move( pyFunc );
}
