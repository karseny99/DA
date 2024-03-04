#pragma once
#include <iostream>
#include "TValue.hpp"
#include "vector.hpp"


using Pair = std::pair<std::string, uint64_t>;

namespace sort
{

void radix_sort( vector::Vector<TValue>& elems );

}    