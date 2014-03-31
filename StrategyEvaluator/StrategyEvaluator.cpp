#include "StrategyEvaluator.hpp"

extern void registerStrategyObject( StrategyEvaluator& strategy );

StrategyEvaluator::StrategyEvaluator( PythonInterpreter::py_ptr& strategy )
  :
  strategy_( strategy ),
  currentDate_( 0 )
{
}

StrategyEvaluator::~StrategyEvaluator() {
}

boost::signals2::connection StrategyEvaluator::connectToOrderHandler( const signal_t::slot_type& subscriber ) {
  return orderSig_.connect( subscriber );
}

void StrategyEvaluator::run( datetime date ) {
  //
  // Package the date into pArgs and call the strategy
  // function from python.
  //

  currentDate_ = date;

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

  //
  // TODO: Investigate other architecture for connecting this class to the
  // Python qsolve.order() function
  //

  //
  // Have to register this object with the Python Module which is in C
  //
  registerStrategyObject( *this );

  //
  // Execute the python strategy function
  //
  if( PyObject_CallObject( strategy_.get(), pArgs ) == NULL ) {
    if( PyErr_Occurred() ) {
      PyErr_Print();
      throw std::invalid_argument( "Python strategy function could not be called" );
    }
  }
}

void StrategyEvaluator::addOrder( const std::string& symbol, long numOfShares ) {
  using namespace std;
  //cout << "Received order for " << symbol << ": " << numOfShares << endl;
  orderSig_( currentDate_, symbol, numOfShares );
}
