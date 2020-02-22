#include "queryengine.h"
using namespace std;
QueryEngine::QueryEngine()
{
}

void QueryEngine :: inputAnalysisHelper(std::stringstream &ss, Query *&q){
    bool isNot = false;
    std::string temp;
    while(ss >> temp){
        if(!isNot){
            if(temp == "NOT"){
                isNot = true;
                q->setHasNot(true);
            }else{
                q->pushSearchWords(temp);
            }
        }else{ // when isNot is true
            q->pushNotWord(temp);
        }
    }
}

Query& QueryEngine :: parseInput(std::string input){
    std::stringstream ss;
    Query* newQuery = new Query;
    // do input anaylsis thing & put them into Query
    if(input.substr(0,2) == "OR"){
        newQuery->setHasAnd(false);
        ss.str(input.substr(2));
        inputAnalysisHelper(ss,newQuery);
    }
    else if(input.substr(0,3) == "AND"){
        newQuery->setHasAnd(true);
        ss.str(input.substr(3));
        inputAnalysisHelper(ss,newQuery);
    }else{
        ss.str(input);
        inputAnalysisHelper(ss,newQuery);
    }
    return *newQuery;
}

void QueryEngine :: rankResult(std::vector<Opinion> & OpinionArr){

    int num;
    int index;
    double relavency;

    std::vector<Opinion> sortedArr;

    if(OpinionArr.size() != 0){


        for(int i = 0; i < OpinionArr.size()-1; ++i){
            index = i;
            relavency = OpinionArr[i].getRelavency();
            for(int j = i + 1; j < OpinionArr.size(); ++j){
                if(OpinionArr[j].getRelavency() > relavency){
                    index = i;
                    relavency = OpinionArr[i].getRelavency();
                }
            }
            sortedArr.push_back(OpinionArr[index]);
            if(sortedArr.size() >= 15) break;
            std::vector<Opinion>::iterator k = OpinionArr.begin();
            for(int l = 0; l < index; ++l) ++k;
            OpinionArr.erase(k);
        }


        for(int i = 0; i < sortedArr.size() && i < 15; ++i){
            std::cout << std::endl << "************" << i+1 << "************" << std::endl;
            sortedArr[i].print(std::cout);
        }
        std::cout << std::endl << "**************************" << std::endl;

        while(true){
            std::cout << "Which one do you want to see? -1 for exit" << std::endl;

            std::cin >> num;

            if(num == -1) break;

            std::cout << sortedArr[num-1].getFirst300Words() << std::endl;
        }
    }

    delete &OpinionArr;
}
