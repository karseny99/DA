#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>

namespace BayesClassificator {


enum class classType {
    DOCUMENT,
    NOT_DOCUMENT
};

class GaussianNaiveBayes {
private:    
    using text_t = std::vector<std::string>;

    std::unordered_map<std::string, int> freqDoc;
    std::unordered_map<std::string, int> freqNotDoc;

    int docWordCount = 0;
    int notDocWordCount = 0;

    int docsEntries = 0;
    int notDocEntries = 0;

public:

    classType predict(const text_t& text);

    void fit(classType type, const text_t& text);

};


}


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
    double probNotDoc = log(notDocEntries / static_cast<double>(docsEntries + notDocEntries));

    for(const std::string& word : text) {
        probNotDoc += log(freqNotDoc[word] + 1e-6);
        probNotDoc -= log(notDocWordCount);
    }

    return (probDoc >= probNotDoc) ? BayesClassificator::classType::DOCUMENT : BayesClassificator::classType::NOT_DOCUMENT;
}
void init() {

    int trainSize, testSize;
    std::cin >> trainSize >> testSize; 

    BayesClassificator::GaussianNaiveBayes gnb;

    for(int i{0}; i < trainSize; ++i) {

        int type; 
        std::cin >> type; 
        std::vector<std::string> text;
        std::string currentString, word;
        std::cin.get(); // skip '\n'
        getline(std::cin, currentString);

        std::stringstream currentStrBuf{currentString};
        while(currentStrBuf >> word) text.emplace_back(word);

        gnb.fit(static_cast<BayesClassificator::classType>(type), text);
    }

    for(int i{0}; i < testSize; ++i) {
        std::vector<std::string> text;
        std::string currentString, word;
        std::cin.get(); // skip '\n'
        getline(std::cin, currentString);
        std::stringstream currentStrBuf{currentString};
        while(currentStrBuf >> word) text.emplace_back(word);

        std::cout << static_cast<int>(gnb.predict(text)) << std::endl;
    }

}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    init();

}