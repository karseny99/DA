#include "../include/classificator.hpp"

void BayesClassificator::BayesTagClassificator::showFrequency() const {
    for(auto [key, value] : fittedTags) {
        std::cout << '[' << key << ']' << ' ' << value.tagEntry << ' ' << value.wordsUnderTag << std::endl;

        for(auto [tag, freq] : value.freq) {
            std::cout << tag << ' ' << freq << std::endl;
        }

    }
}

std::vector<std::pair<std::string, double>>& BayesClassificator::softmax(std::vector<std::pair<std::string, double>>& arr) {
    double maxValue = (*max_element(arr.begin(), arr.end(), [](const auto& lhs, const auto& rhs) {return lhs.second < rhs.second;})).second;

    double sum = 0;
    for(const auto& [name, value] : arr) {
        sum += exp(value - maxValue);
    }

    double offset = maxValue + log(sum);
    for(auto& [name, value] : arr) {
        value = exp(value - offset);
    }
    return arr;
}

std::unordered_map<std::string, size_t> BayesClassificator::BayesTagClassificator::getFrequency(const text_t& text) const {
    std::unordered_map<std::string, size_t> freqDict;
    for(const std::string& word : text) 
        ++freqDict[word];
    
    return freqDict;
}

void BayesClassificator::BayesTagClassificator::fit(const std::vector<std::string>& tags, const text_t& text) {
    std::unordered_map<std::string, size_t> freqDict = getFrequency(text);

    for(const std::string& tag : tags) {
        fittedTags[tag].add(freqDict, text.size());
    }
}

std::vector<std::pair<std::string, double>> BayesClassificator::BayesTagClassificator::predict(const text_t& text) {
    std::vector<std::pair<std::string, double>> tagsProbs;

    for(auto [tagName, tagInfo] : fittedTags) {
        double tagLogProb = log(tagInfo.tagEntry) - log(static_cast<double>(fittedTags.size())); 

        for(const std::string& word : text) {
            tagLogProb += log(tagInfo.freq[word] + 1) - log(static_cast<double>(tagInfo.wordsUnderTag));
        }

        tagsProbs.push_back({tagName, tagLogProb});
    }

    return BayesClassificator::softmax(tagsProbs);
}

std::string BayesClassificator::readTag(std::istream& is) {
    std::string tag;
    while(is.peek() != ',' and is.peek() != '\n') {
        tag += is.get();
    }

    return BayesClassificator::normalize(tag);
}

std::string& BayesClassificator::normalize(std::string& s) {
    for(size_t i{0}; i < s.size(); ++i) {
        if('A' <= s[i] and s[i] <= 'Z') 
            s[i] = (s[i] - 'A' + 'a');
    }
    if(s.back() == '?') s.pop_back();

    return s;
}
