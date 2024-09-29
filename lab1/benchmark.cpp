#include <iostream>
#include <chrono>
#include <algorithm>

#include "sort.hpp"
#include "vector.hpp"
#include "TValue.hpp"

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";


bool cmp(const value::TValue& a, const value::TValue& b) {
    return a.key < b.key;
}

int main() {
    vector::Vector<value::TValue> input(true);
    value::TValue* input_stl = new value::TValue[MAX_ARRAY_SIZE];
    
    std::string key;
    uint64_t value;
    int size = 0;

    while (std::cin >> key >> value) {
        input.push_back(value::TValue(key, value));
        input_stl[size].set(key, value) ;
        ++size;
    }


    std::cout << "Count of lines is " << size << std::endl;

    // Измеряем время работы сортировки подсчётом.
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    sort::radix_sort( input );
    auto end_ts = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();

    // // Измеряем время работы stl сортировки.
    start_ts = std::chrono::system_clock::now();

    std::stable_sort(input_stl, input_stl + size, cmp);
    end_ts = std::chrono::system_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "Counting sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
}