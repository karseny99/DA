#include <iostream>

#include "sort.hpp"


int main()
{
    std::string key;
    uint64_t value;

    TValue* elems = new TValue[(int)1e6];

    int size = 0;
    while (std::cin >> key >> value) {
        elems[size++].set(key, value) ;
    }

    sort::radix_sort( elems, size );

    for (int i = 0 ; i < size ; ++i)
    {
        for(int j = 0; j < key_length; ++j) 
            std::cout << elems[i].key[j];
        std::cout << '\t' << elems[i].value << std::endl;
    }

    return 0;
}