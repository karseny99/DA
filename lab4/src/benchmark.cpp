#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <map>
#include <algorithm>
#include <sstream>
#include <chrono>

using duration_t = std::chrono::milliseconds;

// Boyer–Moore string searching algorithm

// compare pattern with text from right to left
// if letters aren't equal - then use one of two heuristic functions
// functions are called 
// - heuristic of good suffix
// - heuristic of bad letter
// cuz they are heuristic, we choose the one that require less comparing   

namespace utils {
    int binary_search(std::vector<int>& vec, int idx) {
        int l = -1, r = vec.size(); 

        while(r - l != 1) {
            int newIdx = (r + l) / 2;
            if(vec[newIdx] >= idx) {
                r = newIdx;
            } else {
                l = newIdx;
            }
        }
        return l;
    }
}

namespace NBoyerMoore {


    template<class T, class Alph>
    class Boyer_Moore {

        private:

            /*  heuristic of good suffix

                N[j] - longest suffix of substring s[1..j] which is also a suffix of s 
                
                l_[i] - largest position such that suffix of s matches the suffix of s[1..l_[i]] (strong one)
                
                l[i] - longest suffix of s[i..n] which is also a prefix of s
            */
            std::vector<int> N;
            std::vector<int> l_;
            std::vector<int> lp;
            std::map<Alph, std::vector<int>> entries;
            T pattern;

            int skipIfMatched;

            const int NOT_FOUND_LETTER = -1;

        private:

            std::vector<int> z_func(const T& s) {
                int n = s.size();
                std::vector<int> z(n);
                int l = 0, r = 0;

                for(int i = 1; i < n; ++i) {
                    if(i <= r)
                        z[i] = std::min(z[i - l], r - i + 1);
                    while(i + z[i] < n and s[z[i]] == s[i + z[i]]) ++z[i];

                    if(r < i + z[i] - 1) {
                        l = i;
                        r = i + z[i] - 1;
                    }
                }
                return z;
            }

        private:

            void get_letters_entry() {
                std::size_t i = 0;
                for(const Alph& el : pattern) {
                    entries[el].push_back(i++);
                }
            }

            void suffix_equal_to_prefix() {
                int n = pattern.size();
                int i = 0;
                for (int j = n - 1; j >= 0; j--)
                {
                    if (N[j] == j + 1)
                    {
                        while (i <= n - (j + 1))
                            lp[i++] = j + 1;
                    }
                }
            }

            void substr_suffix_equal_suffix() {
                T s_copy(pattern);
                reverse(s_copy.begin(), s_copy.end());
                N = z_func(s_copy);
                reverse(N.begin(), N.end());
            }

            void equal_suffixes() {
                substr_suffix_equal_suffix(); 
                int n = pattern.size();
                for(int j = 0; j < n - 1; ++j) {
                    int i = n - N[j];
                    if(i < n)
                        l_[i] = j;
                }
            }

            int good_suffix_heuristic(int i) {

                // First comparison
                if(i == pattern.size() - 1) 
                    return 0;

                // shifting from mismatched letter
                ++i; 
                if(l_[i] > 0) 
                    return pattern.size() - l_[i] - 1;
                return pattern.size() - lp[i] - 1;
            }

            int bad_character_heuristic(int i, Alph c) {
                int entry = utils::binary_search(entries[c], i);
                // std::cout << '\n' << entry << ' ' << c << '\n';
                if(entry == NOT_FOUND_LETTER) 
                    return i + 1;
                return i - entries[c][entry];
            }

        public:

            Boyer_Moore(const T& _pattern) : pattern(_pattern) {
                lp.resize(pattern.size());
                l_.resize(pattern.size());
                get_letters_entry();
                equal_suffixes();
                suffix_equal_to_prefix();
                skipIfMatched = std::max(1, static_cast<int>(pattern.size() - lp[1]));
            }

            std::vector<int> matches(const T& text) {
                std::vector<int> entry;

                if(text.size() < pattern.size()) {
                    return entry;
                }

                int n = text.size();
                int m = pattern.size();
                int shift = 0;
                bool matched;
                int i = 0;

                while(i < n - m + 1) {
                    matched = true;
                    for(int j = m - 1; j >= 0; --j) {
                        if(pattern[j] != text[i + j]) {
                            shift = std::max({bad_character_heuristic(j, text[i+j]), good_suffix_heuristic(j), 1});
                            matched = false;
                            break;
                        }
                    }

                    if(matched) {
                        entry.push_back(i);
                        shift = skipIfMatched;
                    }

                    i += shift;
                } 

                return entry;
            }
    };

    template<class T, class Alph>
    std::vector<int> boyer_moore_search(const T& text, const T& pattern) {
        Boyer_Moore<T, Alph> bm(pattern);
        return bm.matches(text);
    }
}





signed main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    std::vector<uint32_t> pattern;
    std::vector<uint32_t> text;

    std::string currentLine;
    uint32_t tmp;

    std::getline(std::cin, currentLine, '\n');
    std::istringstream patternStream(currentLine);
    while(patternStream >> tmp) pattern.emplace_back(tmp);

    std::vector<std::pair<int, int>> wordPositions;
    int lineCounter = 1;
    while(std::getline(std::cin, currentLine, '\n')) {
        std::istringstream lineStream(currentLine);

        for(int wordCounter = 1; lineStream >> tmp; ++wordCounter) {
            text.emplace_back(tmp);
            wordPositions.emplace_back(std::make_pair(lineCounter, wordCounter));
        }
        ++lineCounter;
    }
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    std::vector<int> e = NBoyerMoore::boyer_moore_search<std::vector<uint32_t>, uint32_t>(text, pattern);
    auto end_ts = std::chrono::system_clock::now();
    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "Boyer-Moore algorithm: " << stl_sort_ts << " ms" << std::endl;
    // for(auto el : e) {
    //     std::cout << wordPositions[el].first << ", " << wordPositions[el].second << '\n';
    // }
}