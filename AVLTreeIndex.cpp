
#include "avltreeindex.h"

int AVLTreeIndex::getNumOfIndexes(){
    return myTree.getSize();
}

void AVLTreeIndex :: clear(){
    myTree.clear();
}

void AVLTreeIndex::print(){
    myTree.print();
}

void AVLTreeIndex :: write(std::ostream &o){
    myTree.outputHelper(o);
}

void AVLTreeIndex :: insert(const Word& word){ // only use when reading from file
    myTree.insert(word);
}

void AVLTreeIndex::insert(const std::string& word){
    Word newWord(word,currentWorkingOpinion);
    myTree.insert(newWord);
    Word* dup = myTree.getDuplicate();
    if(dup != nullptr){
        if(dup->getReference().back().first != currentWorkingOpinion){
            dup->addNewReference(currentWorkingOpinion);
        }else{
            ++(*dup);
        }
    }
}

const Word& AVLTreeIndex :: find (const Word& word){
    return myTree.find(word);
}










