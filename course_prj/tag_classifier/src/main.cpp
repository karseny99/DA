#include "../include/classificator.hpp"

#include <cassert>
#include <cstring>

void learn(std::ifstream& is, std::ofstream& os) {
    BayesClassificator::BayesTagClassificator btc;

    while(!is.eof()) {
        size_t questionLines;
        is >> questionLines; 

        std::vector<std::string> tags;
        while(1) {
            std::string tag = tools::readWord(is >> std::ws);
            tags.emplace_back(tag);
            
            if(is.peek() == ',') is.get();
            if(is.peek() == '\n') {is.get(); break;}
        }

        std::vector<std::string> text;
        for(size_t i{0}; i < questionLines + 1; ++i) {
            std::string currentString, word;
            getline(is, currentString);

            std::stringstream currentStrBuf{currentString};
            while(currentStrBuf >> word) text.emplace_back(tools::normalize(word));
        }

        btc.fit(tags, text);
    }

    btc << os;
}        

void classify(std::ifstream& is, std::ifstream& stats, std::ofstream& os) {
    BayesClassificator::BayesTagClassificator btc;

    stats >> btc;
    std::vector<std::vector<std::pair<std::string, double>>> predictions;
    while(!is.eof()) {
        size_t questionLines;
        is >> questionLines; 
        is.get();   

        std::vector<std::string> text;
        for(size_t i{0}; i < questionLines + 1; ++i) {
            std::string currentString, word;
            getline(is, currentString);
            std::stringstream currentStrBuf{currentString};
            while(currentStrBuf >> word) text.emplace_back(tools::normalize(word));
        }
        predictions.emplace_back(std::move(btc.predict(text)));
    }

    os << predictions.size() << '\n';
    for(const std::vector<std::pair<std::string, double>>& tagItem : predictions) {
        std::string currentLine;
        for(const auto& [tag, prob] : tagItem) 
            currentLine += tag  /*+ ' '+ std::to_string(prob) */ + ", ";
        currentLine.pop_back();
        currentLine.pop_back();
        os << currentLine;
        os << '\n';
    }
}

int main(int argc, char**argv) {
    assert(argc == 6 or argc == 8);

    if(std::strcmp(argv[1], "learn") == 0) {
        
        assert(std::strcmp(argv[2], "--input") == 0);
        assert(std::strcmp(argv[4], "--output") == 0);

        std::ifstream is(argv[3]);
        std::ofstream os(argv[5]);

        BayesClassificator::BayesTagClassificator btc;

        learn(is, os);

        std::cout << "Learnt on " << argv[3] << std::endl;
        std::cout << "Saved stats in " << argv[5] << std::endl;

    } else if(std::strcmp(argv[1], "classify") == 0) {

        assert(std::strcmp(argv[2], "--stats") == 0);
        assert(std::strcmp(argv[4], "--input") == 0);
        assert(std::strcmp(argv[6], "--output") == 0);

        std::ifstream stats(argv[3]);
        std::ifstream is(argv[5]);
        std::ofstream os(argv[7]);
        
        BayesClassificator::BayesTagClassificator btc;

        classify(is, stats, os);

        std::cout << "Stats imported from " << argv[3] << std::endl;
        std::cout << "Data to classify from " << argv[5] << std::endl;
        std::cout << "Classified data in " << argv[7] << std::endl;

    }
}