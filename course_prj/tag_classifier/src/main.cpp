#include "../include/classificator.hpp"


void init() {

    // TODO : 
    /*
    
    input: 

        num of lines in question

        tag1, tag2, tag3, ...

        head of question
        <n lines of question>
    
    */

    int trainSize, testSize;
    std::cin >> trainSize >> testSize; 

    BayesClassificator::BayesTagClassificator gnb;

    for(int i{0}; i < trainSize; ++i) {

        int type; 
        std::cin >> type; 
        std::vector<std::string> text;
        std::string currentString, word;
        std::cin.get(); // skip '\n'
        getline(std::cin, currentString);

        std::stringstream currentStrBuf{currentString};
        while(currentStrBuf >> word) text.emplace_back(word);

        gnb.fit(std::vector<std::string> tags, text);
    }

    for(int i{0}; i < testSize; ++i) {
        std::vector<std::string> text;
        std::string currentString, word;
        std::cin.get(); // skip '\n'
        getline(std::cin, currentString);
        std::stringstream currentStrBuf{currentString};
        while(currentStrBuf >> word) text.emplace_back(word);

        std::cout << static_cast<int>(gnb.predict(text)) << std::endl;;
    }

}        

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    // init();
}