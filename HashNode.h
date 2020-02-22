//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_HASHNODE_H
#define INC_20S_3353_PA01_HASHNODE_H
template <typename K, typename V>

class HashNode{
public:
    K key;
    V value;

    HashNode(){}
    HashNode(K k, V v) : key(k), value(v){}
    V& getValue(){return value;}
};

#endif //INC_20S_3353_PA01_HASHNODE_H
