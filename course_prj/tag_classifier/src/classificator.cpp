#include "../include/classificator.hpp"

size_t BayesClassificator::BayesTagClassificator::uniqueWordsCount() const {
    return wordEntries.size();
}

std::ifstream& BayesClassificator::operator>>(std::ifstream& is, BayesClassificator::BayesTagClassificator& btc) {

    is >> btc.trials;

    size_t fittedTagCount;
    is >> fittedTagCount;

    for(size_t i{0}; i < fittedTagCount; ++i) {

        std::string tagName;
        size_t wordsUnderTag, tagEntry;

        is >> tagName >> wordsUnderTag >> tagEntry;

        size_t freqCount;
        is >> freqCount;

        std::unordered_map<std::string, size_t> _freq;
        for(size_t k{0}; k < freqCount; ++k) {
            std::string word;
            size_t frequency;
            is >> word >> frequency;

            _freq[word] = frequency;
            btc.wordEntries[word] = true;
        }

        btc.fittedTags.emplace(tagName, BayesTagClassificator::tagInfo(_freq, wordsUnderTag, tagEntry));
    }

    return is;
}

std::ofstream& BayesClassificator::BayesTagClassificator::operator<<(std::ofstream& os) const {
    os << trials << '\n';
    os << fittedTags.size() << '\n';
    for(const auto& [tagName, tagInfo] : fittedTags) {
        os << tagName << ' ' << tagInfo.wordsUnderTag << ' ' << tagInfo.tagEntry << '\n';
        os << tagInfo.freq.size() << '\n';
        for(const auto& [word, freq] : tagInfo.freq) {
            os << word << ' ' << freq << '\n';
        }
    }

    return os;
}

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

std::unordered_map<std::string, size_t> BayesClassificator::BayesTagClassificator::getFrequency(const text_t& text) {
    std::unordered_map<std::string, size_t> freqDict;
    for(const std::string& word : text) {
        ++freqDict[word];
        wordEntries[word] = true;
    }

    return freqDict;
}

void BayesClassificator::BayesTagClassificator::fit(const std::vector<std::string>& tags, const text_t& text) {
    std::unordered_map<std::string, size_t> freqDict = getFrequency(text);

    ++trials;

    for(const std::string& tag : tags) {
        fittedTags[tag].add(freqDict, text.size());
    }
}

std::vector<std::pair<std::string, double>> BayesClassificator::BayesTagClassificator::predict(const text_t& text) {
    std::vector<std::pair<std::string, double>> tagsProbs;

    for(auto [tagName, tagInfo] : fittedTags) {
        double tagLogProb = log(tagInfo.tagEntry) - log(trials); 
        for(const std::string& word : text) {
            tagLogProb += log(tagInfo.freq[word] + 1) - log(tagInfo.wordsUnderTag + 1 * uniqueWordsCount());
        }

        tagsProbs.push_back({tagName, tagLogProb});
    }

    tagsProbs = BayesClassificator::softmax(tagsProbs);
    std::vector<std::pair<std::string, double>> predicted;

    for(auto& [tagName, prob] : tagsProbs) {
        if(prob > 1.5 / fittedTags.size())
            predicted.emplace_back(tagName, prob);
    }

    return predicted;
}

std::string tools::readWord(std::istream& is) {
    std::string tag;
    while(is.peek() != ',' and is.peek() != '\n' and is.peek() != -1) 
        tag += is.get();
    

    return tools::normalize(tag);
}

std::string& tools::normalize(std::string& s) {
    for(size_t i{0}; i < s.size(); ++i) {
        if('A' <= s[i] and s[i] <= 'Z') 
            s[i] = (s[i] - 'A' + 'a');
    }
    if(s.back() == '?' or s.back() == '.' or s.back() == ',') s.pop_back();

    return s;
}
