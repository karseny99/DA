#include "../include/classificator.hpp"

void learn(std::ifstream& is, std::ofstream& os) {

    /*   
    input: 
        num of lines in question
        tag1, tag2, tag3, ...
        head of question
        <n lines of question>   
    */
    BayesClassificator::BayesTagClassificator btc;

    while(!is.eof()) {
        int questionLines;
        is >> questionLines; 

        std::vector<std::string> tags;
        while(1) {
            std::string tag = BayesClassificator::readTag(is >> std::ws);
            tags.emplace_back(tag);
            
            if(is.peek() == ',') is.get();
            if(is.peek() == '\n') {is.get(); break;}
        }

        std::vector<std::string> text;
        for(size_t i{0}; i < questionLines + 1; ++i) {
            std::string currentString, word;
            getline(is, currentString);

            std::stringstream currentStrBuf{currentString};
            while(currentStrBuf >> word) text.emplace_back(BayesClassificator::normalize(word));
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
            while(currentStrBuf >> word) text.emplace_back(BayesClassificator::normalize(word));
        }

        for(auto w : text) {
            std::cout << w << ' ' ;
        }
        std::cout << "hwre" << std::endl;
        predictions.emplace_back(btc.predict(text));
    }

    for(const std::vector<std::pair<std::string, double>>& tagItem : predictions) {
        std::string currentLine;
        for(const auto& [tag, prob] : tagItem) 
            currentLine += tag + ' ' + std::to_string(prob) + ',';
        currentLine.pop_back();
        os << currentLine;
        os << '\n';
    }
    // std::vector<std::string> t{"my", "dog", "is", "very", "funny"};
    // std::vector<std::string> t{"Hello"};
    // std::vector<std::string> t{"I", "like", "to", "walk", "with", "dogs"};
    // std::vector<std::string> t{"I", "like", "to", "play", "footbal"};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::ifstream is("files/in.classify");
    std::ofstream os("files/out.result");
    std::ifstream stats("files/out.stats", std::ios_base::out);

    classify(is, stats, os);
    // is >> btc;
    // btc << os;
    // learn(is, os);
}