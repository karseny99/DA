#include "../include/classificator.hpp"



void BayesClassificator::GaussianNaiveBayes::fit(classType type, const text_t& text) {

    if(type == classType::DOCUMENT) {

        docWordCount += text.size();

        for(std::string word : text) {
            ++freqDoc[word];
        }

    } else {

        notDocWordCount += text.size();

        for(std::string word : text) {
            ++freqNotDoc[word];
        }

    }

}


BayesClassificator::classType BayesClassificator::GaussianNaiveBayes::predict(const text_t& text) {
    // calc doc probability
    double probDoc = probabilityDoc;

    for(const std::string& word : text) {
        probDoc *= (freqDoc[word] + 1);
        probDoc /= docWordCount;
    }

    // calc notDoc prob 
    double probNotDoc = probabilityDoc;

    for(const std::string& word : text) {
        probNotDoc *= (freqNotDoc[word] + 1);
        probNotDoc /= docWordCount;
    }

    return (probDoc >= probNotDoc) ? BayesClassificator::classType::DOCUMENT : BayesClassificator::classType::NOT_DOCUMENT;
}