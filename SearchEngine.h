//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_SEARCHENGINE_H
#define INC_20S_3353_PA01_SEARCHENGINE_H


#include "queryengine.h"
#include "indexhandler.h"
#include "documentparser.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

class SearchEngine{

private:

    char mode;
    QueryEngine qE;
    IndexHandler* iH;
    DocumentParser* dP;

    // funcs called by run
    bool prompt();
    void displayMenu();

    // funcs call to other classes
    void addNewPath();
    void printStats();
    void clearIndex();
    void populateIndex();
    void loadIndex(bool);
    void processNewQuery();

    // GUI helper funcs
    void sleep(int=1);
    void clearScreen(int=50);
    void printFromFile(std::string);

    //only one instance allow
    SearchEngine(const SearchEngine&);
    const SearchEngine& operator=(const SearchEngine&);

public:

    SearchEngine();
    ~SearchEngine();

    void run(); // run when no command line arguments
    void run(std::string&,std::string&); // accpets command line arguments
};


#endif //INC_20S_3353_PA01_SEARCHENGINE_H
