#ifndef _ORDER_FUNCTION_HPP_
#define _ORDER_FUNCTION_HPP_

#include "Python.h"
#include <iostream>
#include <string>
#include <mutex>
#include "StrategyEvaluator.hpp"

void initQSolveModule();
void registerStrategyObject( StrategyEvaluator& strategy );

#endif
