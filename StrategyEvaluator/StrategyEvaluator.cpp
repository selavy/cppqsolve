#include "StrategyEvaluator.hpp"

extern void registerStrategyObject( StrategyEvaluator& strategy );

StrategyEvaluator::StrategyEvaluator( Database& database, PythonInterpreter::py_ptr& strategy )
  :
  database_( database ),
  strategy_( strategy )
{
}

StrategyEvaluator::~StrategyEvaluator() {
}

void StrategyEvaluator::run( datetime date ) {
  //
  // Package the date into pArgs and call the strategy
  // function from python.
  //

  //
  // TODO: Figure out how to handle switching to Python Date
  // PyObject* PyDateTime_FromDateAndTime(int year, int month, int day,
  // int hour, int minute, int second, int usecond)
  //
  PyObject * pArgs = PyTuple_New( 1 );
  if( pArgs == NULL ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::bad_alloc();
  }

  PyObject * pValue = PyInt_FromLong( date );
  if( pValue == NULL ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
    }
    throw std::bad_alloc();
  }

  PyTuple_SetItem( pArgs, 0, pValue );
  registerStrategyObject( *this );
  if( PyObject_CallObject( strategy_.get(), pArgs ) == NULL ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
      throw std::invalid_argument( "Python strategy function could not be called" );
    }
  }
}

void StrategyEvaluator::addOrder( const std::string& symbol, long numOfShares ) {
  using namespace std;
  cout << "Received order for " << symbol << ": " << numOfShares << endl;
}
