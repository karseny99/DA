#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <list>

int main() {
    std::ios::sync_with_stdio(false);
    size_t size;
    std::cin >> size;

    std::vector<long long> nums(size);
    std::vector<long long> tmpActual(size);

    for(size_t i{0}; i < size; ++i) {
        std::cin >> nums[i]; 
        tmpActual[i] = nums[i];
    }

    std::sort(tmpActual.begin(), tmpActual.end());

    std::vector<long long> wrongNums;
    std::vector<long long> actual;

    size_t k{0};
    for(size_t i{0}; i < size; ++i) {
        if(nums[i] != tmpActual[i]) {
            wrongNums.emplace_back(nums[i]);
            actual.emplace_back(nums[i]);
        }
    }

    std::sort(actual.begin(), actual.end());

    size_t paired = 0, nonPaired = 0;

    // 1 3 3 4 2 5 6 
    // 1 2 3 3 4 5 6

    std::unordered_map<long long, size_t> shifts;
    std::unordered_map<size_t, bool> visited;

    for(size_t i{0}; i < wrongNums.size(); ++i) {
        if(wrongNums[i] == actual[i]) {
            continue;
        }

        size_t firstPairPosition = (std::lower_bound(actual.begin(), actual.end(), wrongNums[i]) - actual.begin()) + shifts[wrongNums[i]]; 
        size_t secondPairPosition = (std::lower_bound(actual.begin(), actual.end(), wrongNums[firstPairPosition]) - actual.begin()) + shifts[wrongNums[firstPairPosition]];

        if(secondPairPosition == i) {
            ++paired;
            ++shifts[wrongNums[i]];
            ++shifts[wrongNums[firstPairPosition]];
            std::swap(wrongNums[i], wrongNums[firstPairPosition]);
            continue;
        }

        ++nonPaired;
        // ++shifts[wrongNums[i]];
    }

    // 1 3 2
    // 1 2 3

    // std::cout << nonPaired << ' ' << paired << std::endl;   

    size_t answer = nonPaired + paired;
    if(nonPaired)
        std::cout << answer - 1 << std::endl;
    else 
        std::cout << answer << std::endl;
}