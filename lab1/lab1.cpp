#include <iostream>
#include "sort.hpp"
#include "vector.hpp"
#include "TValue.hpp"


int main() {
    std::string key;
    uint64_t value;


    vector::Vector<TValue> elems(true);

    while(std::cin >> key >> value) {
        elems.push_back(TValue(key, value));
    }

    sort::radix_sort( elems );

    for (int i = 0 ; i < elems.get_size(); ++i) {
        for(int j = 0; j < key_length; ++j) 
            std::cout << elems[i].key[j];
        std::cout << '\t' << elems[i].value << '\n';
    }

    return 0;
}