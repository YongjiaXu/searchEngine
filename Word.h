//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_WORD_H
#define INC_20S_3353_PA01_WORD_H


#include <utility>
#include <string>
#include <vector>


class Word{

private:

    int totalAppearance;

    std::string word;
    std::vector<std::pair<int,int>> references;

public:

    Word() : totalAppearance(0){}

    Word(const Word& w){*this = w;}
    Word(const int tA, const std::string& s) : totalAppearance(tA), word(s){}
    Word(const std::string& s, const int id = 0) : totalAppearance(0), word(s) {addNewReference(id);}

    const Word& operator=(const Word&);
    const Word& operator=(const std::string& s);

    ~Word();

    // getters
    int getToTalAppearance() const;
    const std::string& getWord() const;
    std::vector<std::pair<int,int>> getReference() const;

    // comparing funcs
    bool operator<(const Word&)const;
    bool operator>(const Word&)const;
    bool operator==(const Word&)const;

    // increase frequency
    void operator++();

    // reference pushing funcs
    void addNewReference(const int&);
    void pushReference(const std::pair<int,int>&);

    friend std::ostream& operator<<(std::ostream&, const Word&);
};


#endif //INC_20S_3353_PA01_WORD_H
