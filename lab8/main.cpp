#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <array>

namespace task {
    const size_t sz = 4;

    void solve() {
        size_t size;
        std::cin >> size;

        std::vector<size_t> wrongNums(size);
        std::array<size_t, sz> counting({0});


        for(size_t i{0}; i < size; ++i) {
            std::cin >> wrongNums[i]; 
            ++counting[wrongNums[i]];
        }

        for(size_t i{sz - 1}; i >= 2; --i) 
            counting[i] = counting[i - 1] + counting[i - 2];
        counting[1] = 0;


        size_t swaps = 0;

        // processing only 2 in range of ones
        for(size_t i{0}; i < counting[2]; ++i) {
            if(wrongNums[i] != 2) continue;
            for(size_t j{counting[2]}; j < size; ++j) {
                if(wrongNums[j] == 1) {
                    std::swap(wrongNums[i], wrongNums[j]);
                    ++swaps;
                    break;
                }
            }
        }

        // processing only 3 in range of ones
        for(size_t i{0}; i < counting[2]; ++i) {
            if(wrongNums[i] != 3) continue;
            for(size_t j{size - 1}; j >= counting[2]; --j) {
                if(wrongNums[j] == 1) {
                    std::swap(wrongNums[i], wrongNums[j]);
                    ++swaps;
                    break;
                }
            }
        }
        // now range of ones is actually range of ones


        // processing range of twos consisted only of 2 and 3, skipping 2 
        // In fact, range of thress also consist only of 2 & 3
        for(size_t i{counting[2]}; i < counting[3]; ++i) {
            if(wrongNums[i] == 2) continue;

            for(size_t j{size - 1}; j >= counting[3]; --j) {
                if(wrongNums[j] == 2) {
                    std::swap(wrongNums[i], wrongNums[j]);
                    ++swaps;
                    break;
                }
            }
        }

        std::cout << swaps << '\n';   
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    task::solve();
}