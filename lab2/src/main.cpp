#include "../include/map.hpp"

const std::string SAVE = "Save";
const std::string LOAD = "Load";

char to_lower_case(char c) {
    if('A' <= c and c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

void string_to_lower_case(std::string& s) {
    for(int i = 0; i < s.size(); ++i) {
        s[i] = to_lower_case(s[i]);
    }
}

signed main() {
    std::ios::sync_with_stdio(false);

    char c;
    std::string key;
    uint64_t value;

    NMap::TMap<std::string, uint64_t> mp;

    while(std::cin >> c) {
        if(c == '+') {
            std::cin >> key >> value;
            string_to_lower_case(key);

            if(mp.insert({key, value})) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Exist" << std::endl;
            }
        } else if(c == '-') {
            std::cin >> key;
            // std::transform(key.begin(), key.end(), key.begin(), to_lower_case);
            string_to_lower_case(key);
            
            if(mp.erase(key)) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "NoSuchWord" << std::endl;
            }
        } else if(c == '!') {
            std::string cmd, path;
            std::cin >> cmd >> path;

            if(cmd == SAVE) {
                try {
                    std::ofstream os(path, std::ios::binary);
                    mp.save_to_file(os);
                    std::cout << "OK" << std::endl;
                } catch(...) {
                    std::cout << "ERROR: Cannot write in file" << std::endl;
                }

            } else {
                try {
                    std::ifstream is(path, std::ios::binary);
                    mp.load_from_file(is);
                    std::cout << "OK" << std::endl;
                } catch(...) {
                    std::cout << "ERROR: Cannot load from file" << std::endl;
                }
            }
        } else {
            std::string tmpKey;
            getline(std::cin, tmpKey);
            key = c + tmpKey;
            // std::transform(key.begin(), key.end(), key.begin(), to_lower_case);
            string_to_lower_case(key);

            NMap::TMap<std::string, uint64_t>::TMapIterator ptr = mp.find(key);
            if(ptr == mp.end()) {
                std::cout << "NoSuchWord" << std::endl;
            } else {
                std::cout << "OK: " << *ptr << std::endl;
            }
        }
    }
}