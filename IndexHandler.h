//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_INDEXHANDLER_H
#define INC_20S_3353_PA01_INDEXHANDLER_H


#include "opinion.h"
#include "query.h"
#include "avltreeindex.h"
#include "hashtableindex.h"
#include "word.h"
#include <list>

class DocumentParser;

class IndexHandler
{
private:

    // stats variables
    int totalFrequency;
    int numberOfOpinionsIndexed;
    int averageWordsPerOpinion;
    std::vector<std::string> top50Words;

    // control variables
    bool indexesLoaded;
    bool indexesLoadedFromFile;
    bool whatIndexToUse; // avl when true

    // pointer variables
    DocumentParser* dP;
    IndexInterface* avlIndex;
    IndexInterface* hashIndex;
    IndexInterface* stopWordsTree;

    // indexes.txt (creates in build folder)
    std::string indexPath;

    // priv functions
    std::vector<std::pair<int,int>> findWordFromIndex(Query&);
    IndexHandler(const IndexHandler&);
    const IndexHandler& operator=(const IndexHandler&);

public:

    IndexHandler();
    ~IndexHandler();

    // getters for stats
    int getNumberOfOpinionsIndexed();
    int getAverageWordsPerOpinion();
    bool getIndexesLoaded();
    std::vector<std::string>& getTop50Words();

    // setters
    void setDP(DocumentParser&);
    void setWhatIndexToUse(bool); // true for avltree; false for hashtable
    void setWorkingOpinion(const int);

    // funcs called by SearchEngine
    void print();
    void clearIndex();
    void startGeneratingIndexes();
    std::vector<Opinion>& search(Query&);

    // funcs called by DocumentParser
    void loadWord(const std::string&);
    bool isStopWords(const std::string&);
    const Word& search(const Word&); // retrive word from ds

    // I/O funcs
    void writeIndexToFile();
    void readIndexFromFile();
    void loadStopWords();

};


#endif //INC_20S_3353_PA01_INDEXHANDLER_H
