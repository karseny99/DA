#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>


namespace BayesClassificator {

class BayesTagClassificator {
private:    
    using text_t = std::vector<std::string>;

    double probabilityDoc = 0.5;

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

        tagInfo(std::unordered_map<std::string, size_t> _freq) : freq(_freq) {}
    };

    std::unordered_map<std::string, tagInfo> fittedTags;

    std::unordered_map<std::string, size_t> getFrequency(const text_t& text) const;

public:

    // returns vector of predicted tags
    std::vector<std::pair<std::string, double>> predict(const text_t& text);

    void fit(const std::vector<std::string>& tags, const text_t& text);

};


}
