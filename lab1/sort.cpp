#include "sort.hpp"
#include <iostream>


namespace sort
{

    const int array_digit_size = 16;
    const int key_length = 32;


void radix_sort(TValue *elems, int size) 
{

    TValue *tmpResult = new TValue[size];

    for(int j = key_length - 1; j >= 0; --j)
    {

        int tmp[16];
        for(int k = 0; k < 16; ++k) 
        {
            tmp[k] = 0;
        }

        for(int i = 0; i < size; ++i) 
        {
            if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                ++tmp[elems[i].key[j] - '0'];
            else 
                ++tmp[elems[i].key[j] - 'a' + 10];
        }

        for(int k = 1; k < 16; ++k) {
            tmp[k] += tmp[k - 1];
        }

        for(int i = size - 1; i >= 0; --i) 
        {
            int key;
            if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                key = elems[i].key[j] - '0';
            else    
                key = elems[i].key[j] - 'a' + 10;
            int pos = tmp[key]--;
            tmpResult[pos-1] = elems[i];
        }

        TValue *tmp1 = elems;
        elems = tmpResult;
        tmpResult = tmp1;
    }

    delete[] tmpResult;

}



}
