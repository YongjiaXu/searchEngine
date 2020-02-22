#include "word.h"

const Word& Word::operator=(const Word & w){
    if(this != &w){
        totalAppearance = w.totalAppearance;
        word = w.word;
        references = w.references;
    }
    return *this;
}

const Word& Word :: operator=(const std::string& s){
    if(word != s){ word = s;}
    return *this;
}

Word::~Word(){

}

bool Word::operator<(const Word & w)const{
    return (word < w.word);
}

bool Word::operator>(const Word & w)const{
    return (word > w.word);
}

bool Word::operator==(const Word & w)const{
    return (word == w.word);
}

void Word::addNewReference(const int& x){
    ++totalAppearance;
    references.push_back(std::pair<int,int>(x,1));
}

void Word :: pushReference(const std::pair<int,int>& x){
    references.push_back(x);
}

void Word :: operator++(){
    ++totalAppearance;
    ++(references.back().second);
}

const std::string& Word::getWord() const{
    return word;
}

int Word :: getToTalAppearance() const{
    return totalAppearance;
}

std::vector<std::pair<int,int>> Word::getReference() const{
    return references;
}

std::ostream& operator<<(std::ostream& out, const Word& w){
    std::string space = " ";
    out << w.word << space << std::to_string(w.totalAppearance) << space;
    unsigned long size = w.references.size();
    for(unsigned long i = 0; i < size; ++i){
        out << std::to_string(w.references[i].first) << space << std::to_string(w.references[i].second) << space;
    }
    return out;
}
