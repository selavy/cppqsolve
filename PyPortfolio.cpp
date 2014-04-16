#include "Portfolio.hpp"
#include <iostream>

/*
struct MyStruct {
  void getHolding() { std::cout << "Get Holding" << std::endl; }
};
*/

//
// Expose class to Python
//

using namespace boost::python;

BOOST_PYTHON_MODULE(portfolio)
{
  class_<Portfolio>("Portfolio")
    .def("getHolding", &Portfolio::getHolding )
    ;
}
