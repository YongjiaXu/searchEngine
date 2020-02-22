#include "query.h"

Query::Query()
{
    hasAnd = false;
    hasNot = false;
    searchWordSize = 0;
    notWordSize = 0;
}

//***************************Getters**************************

bool Query :: getHasAnd(){
    return hasAnd;
}

bool Query :: getHasNot(){
    return hasNot;
}

int Query :: getNotWordSize(){
    return notWordSize;
}

int Query :: getSearchWordSize(){
    return searchWordSize;
}

std::vector<std::string> Query :: getNotWord(){
    return notWords;
}

std::vector<std::string> Query :: getSearchWord(){
    return searchWords;
}

//***************************Setters**************************

void Query :: setHasAnd(bool hasAnd){
    this->hasAnd = hasAnd;
}

void Query :: setHasNot(bool hasNot){
    this->hasNot = hasNot;
}

//***************************Push Functions**************************

void Query :: pushNotWord(std::string notWord){
    notWords.push_back(notWord);
    ++notWordSize;
}

void Query :: pushSearchWords(std::string newWord){
    searchWords.push_back(newWord);
    ++searchWordSize;
}

//***************************Print Function**************************

void Query :: print(){
    std::cout << "Query result: " << std::endl;

    for(int i = 0; i < searchWords.size(); ++i){
        std::cout << "SearchWord : " << searchWords[i] << std::endl;
    }
    for(int i = 0; i < notWords.size(); ++i){
        std::cout << "NotWord : " << notWords[i] << std::endl;
    }

    std::cout << "SearchWordSize : " << searchWordSize << std::endl;
    std::cout << "NotWordSize : " << notWordSize << std::endl;
    std::cout << "hasAnd : " << hasAnd << std::endl;
    std::cout << "hasNot : " << hasNot << std::endl;
}



