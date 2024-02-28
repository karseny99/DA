#include <iostream>

#include "vector.hpp"
#include "sort.hpp"

/*
 * int -- ключ;
 * std::string -- значение;
 */
int main()
{
    std::string key;
    uint64_t value;

    Vector<Pair> elems;

    // for(int i  = 0; i < 20; ++i) 
    //     elems.push_back(i);

    while (std::cin >> key >> value)
    {
        elems.push_back( Pair(key, value) );
    }

    sort::radix_sort( elems );

    for (int i = 0 ; i < elems.get_size() ; ++i)
    {
        std::cout << elems[i].first << ' ' << elems[i].second << std::endl;
    }

    return 0;
}
