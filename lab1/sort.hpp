#pragma once

#include <iostream>

const int elemsray_digit_size = 16;
const int key_length = 32;

class TValue{
public:
    char key[key_length];
    uint64_t value; 
    void set(std::string& _key, uint64_t _value) {
        for(int i = 0; i < key_length; ++i) 
            key[i] = _key[i];
        value = _value;
    }
};


using Pair = std::pair<std::string, uint64_t>;

namespace sort
{

void radix_sort( TValue* elems, int size );

void counting_sort( TValue* elems, int idx, int size );

} // namespace sort     