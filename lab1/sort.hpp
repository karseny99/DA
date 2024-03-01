#pragma once

#include <iostream>
#include "vector2.hpp"

using Pair = std::pair<std::string, uint64_t>;

namespace sort
{

void radix_sort( Vector<Pair>& elems );

void counting_sort( Vector<Pair> &elems, int idx );

} // namespace sort