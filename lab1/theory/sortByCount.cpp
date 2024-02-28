#include <bits/stdc++.h>

struct KV {
    int key;
    char value;
};

void countingSort(std::vector<KV>& arr, const int& max_value) {
    std::vector<size_t> tmp(max_value + 1);

    for(size_t i = 0; i < arr.size(); ++i) {
        auto key = arr[i].key;
        ++tmp[key];
    }

    for(size_t i = 1; i < tmp.size(); ++i) {
        tmp[i] += tmp[i - 1];
    }

    std::vector<KV> res(arr.size());
    for(size_t i = arr.size(); i > 0; --i) {
        auto key = arr[i - 1].key;
        auto pos = tmp[key] - 1;
        res[pos] = std::move(arr[i - 1]);
        --tmp[key];
    }

    arr.swap(res);
}

int main() {
    KV kv;

    std::vector<KV> arr;
    int max_value = 0;

    while(std::cin >> kv.key >> kv.value) {
        arr.push_back(kv);
        max_value = std::max(max_value, kv.key);
    }

    countingSort(arr, max_value);


    for(size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i].key << ' ' << arr[i].value << std::endl;
    }

}