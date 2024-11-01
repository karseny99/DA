#include "../include/classificator.hpp"

std::unordered_map<std::string, size_t> BayesClassificator::BayesTagClassificator::getFrequency(const text_t& text) const {
    std::unordered_map<std::string, size_t> freqDict;
    for(const std::string& word : text) 
        ++freqDict[word];
    
    return std::move(freqDict);
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

        double tagProb = tagInfo.tagEntry / static_cast<double>(fittedTags.size()); 

        for(const std::string& word : text) {
            tagProb *= (tagInfo.freq[word] + 1);
            tagProb /= tagInfo.wordsUnderTag;
        }

        tagsProbs.emplace_back(std::make_pair(tagName, tagProb));

    }

    return std::move(tagsProbs);
    // calc doc probability
    // double probDoc = probabilityDoc;

    // for(const std::string& word : text) {
    //     probDoc *= (freqDoc[word] + 1);
    //     probDoc /= docWordCount;
    // }

    // // calc notDoc prob 
    // double probNotDoc = probabilityDoc;

    // for(const std::string& word : text) {
    //     probNotDoc *= (freqNotDoc[word] + 1);
    //     probNotDoc /= docWordCount;
    // }

}