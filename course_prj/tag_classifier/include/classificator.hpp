#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <cmath>

namespace BayesClassificator {

class BayesTagClassificator {
private:    
    using text_t = std::vector<std::string>;
  
    struct tagInfo {
        std::unordered_map<std::string, size_t> freq;
        size_t wordsUnderTag = 0;
        size_t tagEntry = 0;

        void add(std::unordered_map<std::string, size_t> _freq, size_t wordCount) {
            ++tagEntry;
            wordsUnderTag += wordCount;
            for(auto [key, value] : _freq) {
                freq[key] += value;
            }
        }

        tagInfo() = default;
        tagInfo(std::unordered_map<std::string, size_t> _freq) : freq(_freq) {}
        tagInfo(std::unordered_map<std::string, size_t> _freq, size_t _wordsUnderTag, size_t _tagEntry) 
            : freq(_freq)
            , wordsUnderTag(_wordsUnderTag)
            , tagEntry(_tagEntry)
        {}
    };
    
    std::unordered_map<std::string, size_t> getFrequency(const text_t& text);

    std::unordered_map<std::string, tagInfo> fittedTags;

    size_t trials = 0; 

    // for computing num of unique words in train
    std::unordered_map<std::string, bool> wordEntries;

    size_t uniqueWordsCount() const;

public:

    std::ofstream& operator<<(std::ofstream& os) const;

    void showFrequency() const;

    // returns vector of predicted tags
    std::vector<std::pair<std::string, double>> predict(const text_t& text);

    void fit(const std::vector<std::string>& tags, const text_t& text);

    friend std::ifstream& operator>>(std::ifstream& is, BayesClassificator::BayesTagClassificator& btc);

};

std::ifstream& operator>>(std::ifstream& is, BayesClassificator::BayesTagClassificator& btc);

std::vector<std::pair<std::string, double>>& softmax(std::vector<std::pair<std::string, double>>& arr);

std::string readTag(std::istream& is);

std::string& normalize(std::string& s);

}
