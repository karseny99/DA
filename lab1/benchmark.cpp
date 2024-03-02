#include <iostream>
#include <chrono>

#include <algorithm>

#include "sort.hpp"

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

/*
using duration_t = std::chrono::milliseconds;
const std::string DURATION_PREFIX = "ms";
*/

bool cmp(const TValue& a, const TValue& b) {
    return a.key < b.key;
}

int main()
{
    TValue* input = new TValue[(int)1e6];
    TValue* input_stl = new TValue[(int)1e6];
    std::string key;
    uint64_t value;
    int i = 0;
    int size = 0;
    while (std::cin >> key >> value) {
        input[i].set(key, value) ;
        input_stl[i].set(key, value) ;
        ++i;
    }
    size = i;

    std::cout << "Count of lines is " << size << std::endl;

    // Измеряем время работы сортировки подсчётом.
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    sort::radix_sort( input ,size );
    auto end_ts = std::chrono::system_clock::now();
    uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    // // Измеряем время работы stl сортировки.
    start_ts = std::chrono::system_clock::now();

    std::stable_sort(input_stl, input_stl + size, cmp);
    end_ts = std::chrono::system_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "Counting sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
}