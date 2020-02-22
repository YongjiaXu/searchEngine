//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_QUERY_H
#define INC_20S_3353_PA01_QUERY_H


#include <iostream>
#include <vector>
#include <string>

class Query{

private:

    bool hasAnd;
    bool hasNot;

    int notWordSize;
    int searchWordSize;

    std::vector<std::string> notWords;
    std::vector<std::string> searchWords;

public:

    Query();

    // getters
    bool getHasAnd();
    bool getHasNot();
    int getNotWordSize();
    int getSearchWordSize();
    std::vector<std::string> getNotWord();
    std::vector<std::string> getSearchWord();

    // setters
    void setHasAnd(bool);
    void setHasNot(bool);

    // push funcs
    void pushNotWord(std::string);
    void pushSearchWords(std::string);

    void print();
};



#endif //INC_20S_3353_PA01_QUERY_H
