#ifndef _ORDER_ENGINE_HPP_
#define _ORDER_ENGINE_HPP_

#include <iostream>
#include <queue>
#include "QTypes.hpp"

class OrderEngine {
public:
  OrderEngine();
  ~OrderEngine();
private:
  std::queue<order_t> orderQueue_;
};

#endif
