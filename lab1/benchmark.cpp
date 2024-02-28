#include <iostream>
#include <chrono>

#include <algorithm>

#include "sort.hpp"
#include "vector.hpp"


using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

/*
using duration_t = std::chrono::milliseconds;
const std::string DURATION_PREFIX = "ms";
*/

int main()
{
    Vector<Pair> input, input_stl;
    std::string key;
    int64_t value;
    while (std::cin >> key >> value)
    {
        input.push_back( Pair(key, value) );
        input_stl.push_back(Pair(key, value));
    }
    // input_stl = input;

    std::cout << "Count of lines is " << input.get_size() << std::endl;

    // Измеряем время работы сортировки подсчётом.
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    sort::radix_sort( input );
    auto end_ts = std::chrono::system_clock::now();
    uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    // Измеряем время работы stl сортировки.
    start_ts = std::chrono::system_clock::now();
    std::stable_sort(input_stl.begin(), input_stl.end());
    end_ts = std::chrono::system_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "Counting sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
}