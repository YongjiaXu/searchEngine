#include "hashtableindex.h"

HashTableIndex :: HashTableIndex(){
    myTable.setSize(10000);
}

HashTableIndex :: ~HashTableIndex(){

}

int HashTableIndex :: getNumOfIndexes(){
    return myTable.getNumofIndexes();
}


void HashTableIndex :: insert(const Word& word){
    myTable.insert(word.getWord(),word);
}

void HashTableIndex :: insert(const std::string & word){
    Word newWord(word,currentWorkingOpinion);
    myTable.insert(word,newWord);
    Word* dup = myTable.getDup();
    if(dup != nullptr){
        if(dup->getReference().back().first != currentWorkingOpinion){
            dup->addNewReference(currentWorkingOpinion);
        }else{
            ++(*dup);
        }
    }
}

void HashTableIndex :: clear(){
    myTable.clear();
}

void HashTableIndex :: print(){

}

void HashTableIndex :: write(std::ostream &o){
    myTable.print(o);
}

const Word& HashTableIndex :: find(const Word& word){
    return myTable.find(word.getWord());
}






