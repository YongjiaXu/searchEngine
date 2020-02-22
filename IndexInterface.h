//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_INDEXINTERFACE_H
#define INC_20S_3353_PA01_INDEXINTERFACE_H

#include "word.h"
class IndexInterface{

protected:

    int currentWorkingOpinion = 0;

public:

    IndexInterface();

    virtual ~IndexInterface();

    virtual int getNumOfIndexes() = 0;

    void setCurrentWorkingOpinion(const int);

    virtual void print() = 0;
    virtual void clear() = 0;
    virtual void write(std::ostream&) = 0;

    virtual void insert(const std::string&) = 0;
    virtual void insert(const Word&) = 0;

    virtual const Word& find(const Word&) = 0;

};

#endif //INC_20S_3353_PA01_INDEXINTERFACE_H
