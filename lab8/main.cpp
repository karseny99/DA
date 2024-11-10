#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    long long size;
    std::cin >> size;

    std::vector<long long> wrongNums(size);
    std::vector<long long> actual(size);
    // std::vector<long long> nums(size);
    // std::vector<long long> tmpActual(size);

    // for(long long i{0}; i < size; ++i) {
    //     std::cin >> nums[i]; 
    //     tmpActual[i] = nums[i];
    // }


    // std::sort(tmpActual.begin(), tmpActual.end());


    // for(long long i{0}; i < size; ++i) {
    //     if(nums[i] != tmpActual[i]) {
    //         wrongNums.emplace_back(nums[i]);
    //         actual.emplace_back(nums[i]);
    //     }
    // }

    for(long long i{0}; i < size; ++i) {
        std::cin >> wrongNums[i]; 
        actual[i] = wrongNums[i];
    }

    std::sort(actual.begin(), actual.end());

    long long paired = 0, nonPaired = 0;

    for(long long i{0}; i < size; ++i) {
        if(wrongNums[i] == actual[i]) continue;

        for(long long j{i + 1}; j < size; ++j) {

            if(wrongNums[j] == actual[j]) continue;

            if(wrongNums[i] == actual[j] and actual[i] == wrongNums[j]) {
                std::swap(wrongNums[i], wrongNums[j]);
                ++paired;
                break;
            }

        }
    }

    for(long long i{0}; i < size; ++i) {
        if(wrongNums[i] != actual[i]) ++nonPaired;
    }

    // std::cout << nonPaired << ' ' << paired << std::endl;   

    long long answer = nonPaired + paired;
    if(nonPaired > 0)
        std::cout << answer - 1 << std::endl;
    else 
        std::cout << answer << std::endl;
}