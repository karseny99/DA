#include <iostream>
#include "map.hpp"
#include "RBtree.hpp"

const std::string SAVE = "Save";
const std::string LOAD = "Load";

signed main() {

    char c;
    std::string key;
    uint64_t value;

    NMap::TMap<std::string, uint64_t> mp;

    while(std::cin >> c) {
        if(c == '+') {
            std::cin >> key >> value;
            if(mp.insert({key, value})) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Exist" << std::endl;
            }
        } else if(c == '-') {
            std::cin >> key;
            if(mp.erase(key)) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "NoSuchWord" << std::endl;
            }
        } else if(c == '!') {
            std::string cmd, path;
            std::cin >> cmd >> path;

            if(cmd == SAVE) {
                std::ofstream os(path, std::ios::binary);
                mp.save_to_file(os);
            } else {
                std::ifstream is(path, std::ios::binary);
                mp.load_from_file(is);
            }

        }
    }
}