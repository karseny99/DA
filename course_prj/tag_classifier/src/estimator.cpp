#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../include/classificator.hpp"

namespace metrics {

class estimator {

private:

    std::vector<std::unordered_set<std::string>> answer;
    std::vector<std::unordered_set<std::string>> prediction;
    std::vector<std::string> distinctClasses;
    
    struct classStats {
        size_t tp = 0;
        size_t fp = 0;
        size_t fn = 0;
    };

    std::unordered_map<std::string, classStats> classesStats;

    std::unordered_map<std::string, double> precisions;
    std::unordered_map<std::string, double> recalls;

public:

    estimator(std::vector<std::unordered_set<std::string>>& _answer, 
        std::vector<std::unordered_set<std::string>>& _prediction,
        std::vector<std::string>& _distinctClasses)
        : answer(_answer)
        , prediction(_prediction)
        , distinctClasses(_distinctClasses)
    {
        assert(_answer.size() == _prediction.size());
    }

    /* 
        return pair<precision, recall>, 
        metrics are macro-averaged: useful if distinctClasses are disbalanced
    */
    std::pair<double, double> getTotalMetrics() {

        // computing precisions and recall for distinct distinctClasses
        getMetricsForEach();

        double precisionMacroAveraging = 0;
        double recallMacroAveraging = 0;

        for(const std::string& cls : distinctClasses) {
            precisionMacroAveraging += precisions[cls];
            recallMacroAveraging += recalls[cls];
        }

        return {precisionMacroAveraging / distinctClasses.size(), recallMacroAveraging / distinctClasses.size()};
    }

private:

    void getMetricsForEach() {
        getStatsForClass();

        for(const std::string& cls : distinctClasses) {
            // skip unused classes in answer
            if(classesStats[cls].tp + classesStats[cls].fp == 0) continue;
            if(classesStats[cls].tp + classesStats[cls].fn == 0) continue; 

            precisions[cls] = classesStats[cls].tp / static_cast<double>(classesStats[cls].tp + classesStats[cls].fp);
            recalls[cls] = classesStats[cls].tp / static_cast<double>(classesStats[cls].tp + classesStats[cls].fn);
        }
    }

    void getStatsForClass() {
    for(const std::string& cls : distinctClasses) {
        for(size_t line{0}; line < answer.size(); ++line) {

            if(answer[line].contains(cls) and prediction[line].contains(cls))
                ++classesStats[cls].tp;
            else if(!answer[line].contains(cls) and prediction[line].contains(cls))
                ++classesStats[cls].fp;
            else 
                ++classesStats[cls].fn;
        }
    }
}



};

}

int main(int argc, char** argv) {
    assert(argc == 5);

    assert(std::strcmp(argv[1], "--answer") == 0);
    assert(std::strcmp(argv[3], "--prediction") == 0);

    /*  file-answer description

        <num of distinct distinctClasses>
        <distinct distinctClasses: <class1>, <class2>, ..., <classn>>
        <num of lines with output classificator>
        <<class1>, <class2>, ...>
        ...
        <<class1>, <class2>, ...>
    */
    std::ifstream isAns(argv[2]);

    std::vector<std::string> distinctClasses;
    size_t distinctsSize;

    isAns >> distinctsSize;

    for(size_t k{0}; k < distinctsSize; ++k) {
        std::string className;
        isAns >> className;

        distinctClasses.emplace_back(className);
    }

    size_t numOfLines;
    isAns >> numOfLines;

    std::vector<std::unordered_set<std::string>> answer(numOfLines);

    for(size_t line{0}; line < numOfLines; ++line) {

        while(1) {
            std::string className = BayesClassificator::readTag(isAns >> std::ws);
            answer[line].insert(className);
            if(isAns.peek() == ',') isAns.get();
            if(isAns.eof() or isAns.peek() == '\n') {isAns.get(); break;}
            
        }

    }

    /*  file-prediction descriptio

        <num of lines with output classificator>
        <<class1>, <class2>, ...>
        ...
        <<class1>, <class2>, ...>
    */
    std::ifstream isPred(argv[4]);

    isPred >> numOfLines;

    std::vector<std::unordered_set<std::string>> prediction(numOfLines);

    for(size_t line{0}; line < numOfLines; ++line) {

        while(1) {
            std::string className = BayesClassificator::readTag(isPred >> std::ws);
            prediction[line].insert(className);
            
            if(isPred.peek() == ',') isPred.get();
            if(isPred.eof() or isPred.peek() == '\n') {isPred.get(); break;}
        }

    }
    metrics::estimator estimator(answer, prediction, distinctClasses);
    auto [precision, recall] = estimator.getTotalMetrics();

    std::cout << precision << ' ' << recall << std::endl;
}