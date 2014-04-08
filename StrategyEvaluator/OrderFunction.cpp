#include "OrderFunction.hpp"

static PyObject*
qsolve_order(PyObject *self, PyObject *args);

static StrategyEvaluator * strategy_ = NULL;
static std::mutex strategyMutex;

static PyMethodDef QsolveMethods[] = {
  {"order", qsolve_order, METH_VARARGS, "Place an order for a symbol"},
  { NULL, NULL, 0, NULL } /* sentinel value */
};

void initQSolveModule(void) {
  Py_InitModule( "qsolve", QsolveMethods );
}

static PyObject*
qsolve_order(PyObject *self, PyObject *args) {
  using namespace std;
  
  const char * szSymbolName;
  long numShares;

  // cout << "Inside order function" << endl;
  if( !PyArg_ParseTuple( args, "sl:order",  &szSymbolName, &numShares ) ) {
    return NULL;
  }

  if( PyErr_Occurred() ) {
    PyErr_Print();
    return NULL;
  }

  string symbolName( szSymbolName );
  if( strategy_ )
    strategy_->addOrder( symbolName, numShares );
  Py_RETURN_NONE;
}

void registerStrategyObject( StrategyEvaluator& strategy ) {
  strategyMutex.lock();
  strategy_ = &strategy;
  strategyMutex.unlock();
}

void unregisterCurrentStrategyObject() {
  strategyMutex.lock();
  strategy_ = NULL;
  strategyMutex.unlock();
}
