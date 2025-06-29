#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = unsigned int;
using size_type = std::size_t; //do usunięcia?

using Time = unsigned int;
using TimeOffset = unsigned int;

using ProbabilityGenerator = std::function<double()>; // tu zależy co zwraca funkcja losująca

#endif //NETSIM_TYPES_HPP
