#include "../include/classificator.hpp"




void BayesClassificator::GaussianNaiveBayes::fit(classType type, const text_t& text) {

    if(type == classType::DOCUMENT) {

        docWordCount += text.size();

        for(std::string word : text) {
            ++freqDoc[word];
        }

        ++docsEntries;

    } else {

        notDocWordCount += text.size();

        for(std::string word : text) {
            ++freqNotDoc[word];
        }

        ++notDocEntries;
    }

}


BayesClassificator::classType BayesClassificator::GaussianNaiveBayes::predict(const text_t& text) {
    // calc doc probability
    double probDoc = log(docsEntries / static_cast<double>(docsEntries + notDocEntries));

    for(const std::string& word : text) {
        probDoc += log(freqDoc[word] + 1e-6);
        probDoc -= log(docWordCount);
    }

    // calc notDoc prob 
    double probNotDoc = (notDocEntries / static_cast<double>(docsEntries + notDocEntries));

    for(const std::string& word : text) {
        probNotDoc += log(freqNotDoc[word] + 1e-6);
        probNotDoc -= log(notDocWordCount);
    }

    return (probDoc >= probNotDoc) ? BayesClassificator::classType::DOCUMENT : BayesClassificator::classType::NOT_DOCUMENT;
}