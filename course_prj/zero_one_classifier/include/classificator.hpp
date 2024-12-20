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
