#include "sort.hpp"
#include <iostream>
#include "vector2.hpp"

namespace sort
{

    const int array_digit_size = 16;
    const int key_length = 32;

void radix_sort( Vector<Pair>& elems ) {
    
    if(elems.empty())
        return;

    for(int i = key_length - 1; i >= 0; --i) {
        counting_sort(elems, i);
    }

    
}

void counting_sort( Vector<Pair>& elems, int idx )
{

    if(idx < 0) 
        throw std::logic_error("Error: wrong index " + idx);

    if (elems.empty())
    {
        return;
    } 


    Vector<int> tmp(array_digit_size);

    for(int i = 0; i < array_digit_size; ++i)
        tmp[i] = 0;

    for (int i = 0; i < elems.size(); ++i) {
        if('0' <= elems[i].first[idx] and elems[i].first[idx] <= '9')
            ++tmp[elems[i].first[idx] - '0'];
        else if('a' <= elems[i].first[idx] and elems[i].first[idx] <= 'f')
            ++tmp[elems[i].first[idx] - 'a' + 10];
        else 
            throw std::logic_error("Error: wrong hex digit " + elems[i].first[idx]);
    }

    
    for (int i = 1; i < tmp.size(); ++i) {
        tmp[i] += tmp[i-1];
    } 
    
    Vector<Pair> result( elems.size() );
    for (int i = elems.size() - 1; i >= 0; --i ) {
        int key;
        if('0' <= elems[i].first[idx] and elems[i].first[idx] <= '9')
            key = elems[i].first[idx] - '0';
        else    
            key = elems[i].first[idx] - 'a' + 10;

        int pos = tmp[key]--;
        // if(pos >= elems.size()) 
        result[pos-1] = elems[i];
    }

    int i = 0;
    for(auto el : result) 
        elems[i++] = el;
    // std::swap(elems, result);
}

}
