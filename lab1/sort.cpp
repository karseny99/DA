#include "sort.hpp"
#include <iostream>


namespace sort {

    const int array_digit_size = 16;
    const int key_length = 32;

    void radix_sort( vector::Vector<value::TValue>& elems ) {

        vector::Vector<value::TValue> tmpResult(elems.get_size() + 1, true);

        for(int j = key_length - 1; j >= 0; --j) {

            int tmp[16];
            for(int k = 0; k < array_digit_size; ++k) {
                tmp[k] = 0;
            }

            for(int i = 0; i < elems.get_size(); ++i) {
                if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                    ++tmp[elems[i].key[j] - '0'];
                else 
                    ++tmp[elems[i].key[j] - 'a' + 10];
            }

            for(int k = 1; k < 16; ++k) {
                tmp[k] += tmp[k - 1];
            }

            for(int i = elems.get_size() - 1; i >= 0; --i) {
                int key;
                if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                    key = elems[i].key[j] - '0';
                else    
                    key = elems[i].key[j] - 'a' + 10;

                int pos = tmp[key]--;
                tmpResult[pos-1] = elems[i];
            }

            vector::swap(elems, tmpResult);
        }

        tmpResult.~Vector();

    }

}
