//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_HASHTABLEINDEX_H
#define INC_20S_3353_PA01_HASHTABLEINDEX_H


#include "indexinterface.h"
#include "hashtable.h"
#include "word.h"

class HashTableIndex : public IndexInterface
{
private:
    HashTable<std::string,Word> myTable;
public:

    HashTableIndex();
    ~HashTableIndex();

    int getNumOfIndexes();

    void insert(const Word&);
    void insert(const std::string&);

    void clear();
    void print();
    void write(std::ostream&);

    const Word& find(const Word&);

};

#endif //INC_20S_3353_PA01_HASHTABLEINDEX_H
