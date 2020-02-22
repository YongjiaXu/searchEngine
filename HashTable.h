//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_HASHTABLE_H
#define INC_20S_3353_PA01_HASHTABLE_H
#include <iostream>
#include <vector>

template<typename K, typename V>

class HashTable{
private:

    // HashNode here
    class HashNode{
    public:
        K key;
        V value;

        HashNode(){}
        HashNode(K k, V v) : key(k), value(v){}
    };

    int numOfIndexes;

    std::vector<std::vector<HashNode>> table;

    V* dup;
    V* lastBoi;

    int hashInt(const K& k) const{
        signed long hash = 0;
        for(unsigned long i = 0; i < k.size(); ++i){
            hash = hash * 101 + k[i];
        }
        hash = hash % table.size();
        if(hash < 0){
            hash += table.size();
        }
        return hash;
    }

    void rehash(){
        std::vector<std::vector<HashNode>> oldList = table;
        table.resize(2 * table.size());
        for(unsigned long j = 0; j < table.size(); ++j){
            table[j].clear();
        }

        for(unsigned long i = 0; i < oldList.size(); ++i){
            for(unsigned long k = 0; k < oldList[i].size(); ++k){
                insert(oldList[i][k].key,oldList[i][k].value);
            }
        }
        numOfIndexes /= 2;
    }

public:


    HashTable() : numOfIndexes(0) , dup(nullptr){}

    const V& find(const K& k) const{
        for(unsigned long i = 0; i < table[hashInt(k)].size(); ++i){
            if(table[hashInt(k)][i].key == k){
                return table[hashInt(k)][i].value;
            }
        }
        return *lastBoi;
    }

    bool isEmpty() const{
        for(unsigned long i = 0; i < table.size(); ++i){
            if(table[i].size() != 0) return false;
        }
        return true;
    }

    void clear(){
        for(unsigned long i = 0; i < table.size(); ++i) table[i].clear();
    }

    void insert(const K& k, const V& v){
        dup = nullptr;
        int loc = hashInt(k);
        HashNode b(k,v);
        for(unsigned long i = 0; i < table[loc].size(); ++i){
            if(table[loc][i].key == k){
                dup = &(table[loc][i].value);
                return;
            }
        }
        table[loc].push_back(b);
        lastBoi = &(b.value);
        ++numOfIndexes;
        if(table[loc].size() >= 15) rehash();
    }

    V* getDup() const{
        return dup;
    }

    void setSize(int size){
        for(int i = 0; i < size; ++i){
            std::vector<HashNode> v;
            table.push_back(v);
        }
    }

    int getNumofIndexes() const{
        return numOfIndexes;
    }

    void print(std::ostream& out) const{
        for(unsigned long i = 0; i < table.size(); ++i){
            for(unsigned long j = 0; j < table[i].size(); ++j){
                out << "value: " << table[i][j].value << " key: " << table[i][j].key << std::endl;
            }
        }
    }
};
#endif //INC_20S_3353_PA01_HASHTABLE_H
