#include "../include/classificator.hpp"

#include <sstream>


void train() {

    /*   
    input: 
        num of lines in question
        tag1, tag2, tag3, ...
        head of question
        <n lines of question>   
    */
    BayesClassificator::BayesTagClassificator gnb;

    while(!std::cin.eof()) {
        int questionLines;
        std::cin >> questionLines; 

        std::vector<std::string> tags;
        while(1) {
            std::string tag = BayesClassificator::readTag(std::cin >> std::ws);
            tags.emplace_back(tag);
            
            if(std::cin.peek() == ',') std::cin.get();
            if(std::cin.peek() == '\n') {std::cin.get(); break;}
        }

        // for(auto word : tags) {
        //     std::cout << word << std::endl;
        // }

        std::vector<std::string> text;
        for(size_t i{0}; i < questionLines + 1; ++i) {
            std::string currentString, word;
            getline(std::cin, currentString);

            std::stringstream currentStrBuf{currentString};
            while(currentStrBuf >> word) text.emplace_back(BayesClassificator::normalize(word));
        }

        // for(auto word : text) {
        //     std::cout << word << std::endl;
        // }

        gnb.fit(tags, text);
    }

    // gnb.showFrequency();

    // std::vector<std::string> t{"my", "dog", "is", "very", "funny"};
    // std::vector<std::string> t{"Hello"};
    // std::vector<std::string> t{"I", "like", "to", "walk", "with", "dogs"};
    std::vector<std::string> t{"I", "like", "to", "play", "footbal"};
    auto tmp = gnb.predict(t);
    for(auto [tag, prob] : tmp) {
        std::cout << tag << ' ' << prob << std::endl;
    }
}        

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    train();
}