#include "opinion.h"

Opinion :: Opinion(){
    relavency = 0;
}

void Opinion :: setRelavency(const double & n){
    relavency = n;
}

void Opinion :: setJudge(const std::string& s){
    judge = s;
}

void Opinion :: setCreatedDate(const std::string& s){
    createdDate = s;
}

void Opinion :: setModifiedDate(const std::string& s){
    modifiedDate = s;
}

void Opinion :: setFirst300Words(const std::string& s){
    first300Words = s;
}

void Opinion :: pushNewParty(const std::string& s){
    parties.push_back(s);
}

void Opinion :: pushNewRelaventWords(const std::pair<std::string,int>& p){
    relaventWords.push_back(p);
}
double Opinion :: getRelavency(){
    return relavency;
}

std::string& Opinion :: getJudge(){
    return judge;
}

std::string& Opinion :: getCreatedDate(){
    return createdDate;
}

std::string& Opinion :: getModifiedDate(){
    return modifiedDate;
}

std::string& Opinion :: getFirst300Words(){
    return first300Words;
}

std::vector<std::string>& Opinion :: getParties(){
    return parties;
}

std::vector<std::pair<std::string,int>>& Opinion :: getRelaventWords(){
    return relaventWords;
}

void Opinion :: print(std::ostream& out){
    out << std::endl << "Date created: " << createdDate << std::endl;
    out << "Date modified: " << modifiedDate << std::endl;
    out << "List of Parties: ";
    for (int i = 0; i < parties.size(); ++i) out << parties[i] << ' ';
    out << std::endl << "Judge: " << judge << std::endl;
}
