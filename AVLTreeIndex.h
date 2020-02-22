//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_AVLTREEINDEX_H
#define INC_20S_3353_PA01_AVLTREEINDEX_H


#include <iostream>
#include "indexinterface.h"
#include "avltree.h"

class AVLTreeIndex : public IndexInterface{

private:

    AVLTree<Word> myTree;

public:

    AVLTreeIndex(){}
    ~AVLTreeIndex(){}

    int getNumOfIndexes();

    void clear();
    void print();
    void write(std::ostream&);

    void insert(const Word&);
    void insert(const std::string&);

    const Word& find(const Word&);
};


#endif //INC_20S_3353_PA01_AVLTREEINDEX_H
