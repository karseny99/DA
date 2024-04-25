#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "map.hpp"
#include "RBTree.hpp"

using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using graph = vector< vector<int> >;

#define all(x) x.begin(), x.end()

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

const int MOD = 1e9 + 7;
const ll INF = 1e18;
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; ++i) cin >> a[i];


}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    map<string, uint64_t> mp;
    std::string key;
    uint64_t value;

    vector<pair<string, uint64_t>> vec;
    while (std::cin >> key >> value) {
        mp[key] = value;
        vec.push_back({key, value});
    }
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    for(auto& e : vec) {
        mp[e.first];
    }
    auto end_ts = std::chrono::system_clock::now();
    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    NMap::TMap<string, uint64_t> mp1;
    for(auto& e : vec) {
        NMap::TPair ins = {e.first, e.second}; 
        mp1.insert(ins);
    }

    std::chrono::time_point<std::chrono::system_clock> start_ts1 = std::chrono::system_clock::now();
    for(auto& e : vec) {
        *(mp1.find(e.first));
    }
    auto end_ts1 = std::chrono::system_clock::now();
    // std::cout << end_ts1. << ' ' << start_ts1 << std::endl;
    uint64_t my_map = std::chrono::duration_cast<duration_t>( end_ts1 - start_ts1 ).count();

    std::cout << "STL map time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "My map time: " << my_map << DURATION_PREFIX << std::endl;
}


















