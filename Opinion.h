//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_OPINION_H
#define INC_20S_3353_PA01_OPINION_H


#include <string>
#include <vector>
#include <utility>
#include <iostream>

class Opinion{

private:

    double relavency;

    std::string judge;
    std::string createdDate;
    std::string modifiedDate;
    std::string first300Words;

    std::vector<std::string> parties;
    std::vector<std::pair<std::string,int>> relaventWords;

public:

    Opinion();

    // setters
    void setRelavency(const double&);
    void setJudge(const std::string&);
    void setCreatedDate(const std::string&);
    void setModifiedDate(const std::string&);
    void setFirst300Words(const std::string&);

    // vector pushers
    void pushNewParty(const std::string&);
    void pushNewRelaventWords(const std::pair<std::string,int>&);

    // getters
    double getRelavency();
    std::string& getJudge();
    std::string& getCreatedDate();
    std::string& getModifiedDate();
    std::string& getFirst300Words();
    std::vector<std::string>& getParties();
    std::vector<std::pair<std::string,int>>& getRelaventWords();

    void print(std::ostream&);
};


#endif //INC_20S_3353_PA01_OPINION_H
