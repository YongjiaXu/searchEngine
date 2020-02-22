//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_DOCUMENTPARSER_H
#define INC_20S_3353_PA01_DOCUMENTPARSER_H


#include "word.h"
#include "opinion.h"
#include "query.h"
#include "porter2_stemmer.h"
#include <list>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class IndexHandler;

class DocumentParser
{
private:

    // data member
    int fileNum;
    int currentPath; // used as an index

    IndexHandler* iH;

    std::string pathsPath; // paths.txt
    std::vector<std::string> paths; // paths store in vector

    // checkers
    bool validChar(const char&);
    bool validNum(const char&);
    bool findIfEmpty(const std::string&);

    // I/O funcs
    void storePathsToFile();
    void readPathsFromFile();

    // parsing funcs
    int stripFileID(const std::string&);
    void trimAndStem(std::string&);
    void parseThisPath(std::string&);
    void textParsing(std::string&, bool);

    // info extracting funcs
    void timeExtraction(std::string&);
    void htmlAnalysis(Opinion&, std::string&);
    std::string findJudge(std::string&);
    std::string first300WordsExtraction(std::string&,bool,int&);

    // only one instance allow
    DocumentParser(const DocumentParser&);
    const DocumentParser& operator=(const DocumentParser&);

public:

    DocumentParser();
    ~DocumentParser();

    // getter
    int getFileNum();
    bool hasNewPath();

    // setters
    void setiH(IndexHandler&);
    void resetCurrentPath();

    // parsing starters
    void startParsing(); // parse every path stored in paths & push to indexHandler
    void directParse(std::string&,std::string&); // parsing starts by command line arguments

    // func called by SearchEngine
    void addNewPath(std::string);

    // parsing Opinion funcs
    std::string searchOpinion(int);
    Opinion& parseOpinion(std::string, Query&,std::pair<int,int>&,double);
    std::vector<Opinion>& findOpinions(std::vector<std::pair<int,int>>&,Query&);
};



#endif //INC_20S_3353_PA01_DOCUMENTPARSER_H
