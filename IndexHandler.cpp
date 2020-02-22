#include "indexhandler.h"
#include "documentparser.h"

IndexHandler::IndexHandler(){
    dP = nullptr;
    totalFrequency = 0;
    numberOfOpinionsIndexed = 0;
    averageWordsPerOpinion = 0;
    avlIndex = new AVLTreeIndex;
    stopWordsTree = new AVLTreeIndex;
    hashIndex = new HashTableIndex;
    indexesLoadedFromFile = false;
    indexesLoaded = false;
    whatIndexToUse = false;
    indexPath = "indexes.txt";
    loadStopWords();
}

IndexHandler :: ~IndexHandler(){
    delete avlIndex;
    delete stopWordsTree;
    delete hashIndex;
}

//***************************Private Helper Function**************************

std::vector<std::pair<int,int>> IndexHandler :: findWordFromIndex(Query& request){

    bool foundSame = false;

    std::vector<std::string> searchWords = request.getSearchWord();
    std::vector<std::string> notWords = request.getNotWord();
    std::vector<std::pair<int,int>> returningReferences;
    std::vector<std::vector<std::pair<int,int>>> workingList;
    std::vector<std::vector<std::pair<int,int>>> notWordList;

    for(int i = 0; i < searchWords.size(); ++i){
        Porter2Stemmer::stem(searchWords[i]);
        Word searchWord(searchWords[i]);
        Word foundWord = (whatIndexToUse) ? avlIndex->find(searchWord) : hashIndex->find(searchWord);
        if(foundWord == searchWord){
            workingList.push_back(foundWord.getReference());
        }else{
            std::cout << searchWord.getWord() << " is not found " << std::endl;
        }
    }

    if(workingList.size() > 1){
        if(request.getHasAnd()){ // Case AND
            for(int i = 0; i < workingList.size()-1; ++i){
                if(foundSame){
                    for(int j = 0; j < returningReferences.size(); ++j){
                        for(int k = 0; k < workingList[i+1].size(); ++k){
                            if(returningReferences[j].first == workingList[i+1][k].first){
                                returningReferences.push_back(workingList[i][j]);
                                break;
                            }
                        }
                    }
                }
                for(int j = 0; j < workingList[i].size(); ++j){
                    for(int k = 0; k < workingList[i+1].size(); ++k){
                        if(workingList[i][j].first == workingList[i+1][k].first){
                            returningReferences.push_back(workingList[i][j]);
                            foundSame = true;
                            break;
                        }
                    }
                }
            }
        }
        else{ // Case OR
            for(int i = 0; i < workingList.size()-1; ++i){
                for(int j = 0; j < workingList[i].size(); ++j){
                    returningReferences.push_back(workingList[i][j]);
                    for(int k = 0; k < workingList[i+1].size(); ++k){
                        if(returningReferences.back().first == workingList[i+1][k].first){
                            returningReferences.pop_back();
                            break;
                        }
                    }
                }
            }
            for(int i = 0; i < workingList.back().size(); ++i) returningReferences.push_back(workingList.back()[i]);
        }
    }else if(workingList.size() == 1)
        for(int i = 0; i < workingList[0].size(); ++i) returningReferences.push_back(workingList[0][i]);

    // clear NOT words index
    if(request.getHasNot()){
        for(int i = 0; i < notWords.size(); ++i){
            Porter2Stemmer::stem(searchWords[i]);
            Word searchWord(notWords[i]);
            Word foundWord = (whatIndexToUse) ? avlIndex->find(searchWord) : hashIndex->find(searchWord);
            if(foundWord == searchWord){
                notWordList.push_back(foundWord.getReference());
            }else{
                std::cout << searchWord.getWord() << " is not found " << std::endl;
            }
        }
        for(std::vector<std::pair<int,int>>::iterator i = returningReferences.begin(); i != returningReferences.end(); ++i){
            for(int j = 0; j < notWordList.size(); ++j){
                for(int k = 0; k < notWordList[j].size(); ++k){
                    if(returningReferences.size() == 0) break;
                    if(i->first == notWordList[j][k].first){
                        returningReferences.erase(i);
                    }
                }
                if(returningReferences.size() == 0) break;
            }
            if(returningReferences.size() == 0) break;
        }
    }

    if(returningReferences.size() == 0) std::cout << "cannot find any opinion with this deman" << std::endl;

    return returningReferences;
}

//***************************Getters for stats**************************

int IndexHandler :: getNumberOfOpinionsIndexed(){
    (whatIndexToUse)? numberOfOpinionsIndexed = avlIndex->getNumOfIndexes() : numberOfOpinionsIndexed = hashIndex->getNumOfIndexes();
    return numberOfOpinionsIndexed;
}

int IndexHandler :: getAverageWordsPerOpinion(){
    return totalFrequency/dP->getFileNum();
}

bool IndexHandler :: getIndexesLoaded(){
    return indexesLoaded;
}

std::vector<std::string>& IndexHandler :: getTop50Words(){
    return top50Words;
}

//***************************Setters**************************

void IndexHandler :: setDP(DocumentParser &dP){
    this->dP = &dP;
}

void IndexHandler :: setWhatIndexToUse(bool isAVL){
    whatIndexToUse = isAVL;
}

void IndexHandler :: setWorkingOpinion(const int id){
    avlIndex->setCurrentWorkingOpinion(id);
    hashIndex->setCurrentWorkingOpinion(id);
}

//***************************Fucntions called by SearchEngine**************************

void IndexHandler :: print(){
    (whatIndexToUse)? avlIndex->print() : hashIndex->print();
    std::cout << std::endl;
}

void IndexHandler :: clearIndex(){
    numberOfOpinionsIndexed = 0;
    averageWordsPerOpinion = 0;
    top50Words.clear();
    avlIndex->clear();
    hashIndex->clear();
    indexesLoadedFromFile = false;
}

void IndexHandler :: startGeneratingIndexes(){
    if(indexesLoaded) clearIndex();
    if(!indexesLoadedFromFile) readIndexFromFile();
    if(dP->hasNewPath()) dP->startParsing();
    writeIndexToFile();
    indexesLoaded = true;
}

std::vector<Opinion>& IndexHandler :: search(Query& request){
    std::vector<std::pair<int,int>> list = findWordFromIndex(request);
    std::vector<Opinion>* opinionList = &((*dP).findOpinions(list,request));
    delete &request;
    return *opinionList;
}

//***************************Fucntions called by DocumentParser**************************

void IndexHandler :: loadWord(const std::string& word){
    (whatIndexToUse)? avlIndex->insert(word) : hashIndex->insert(word);
}

bool IndexHandler :: isStopWords(const std::string & word){
    Word searchThisWord(word);
    return (searchThisWord == stopWordsTree->find(searchThisWord));
}

const Word& IndexHandler :: search(const Word& w){
    if(whatIndexToUse){
        return avlIndex->find(w);
    }else{
        return hashIndex->find(w);
    }
}

//***************************I/O Functions**************************

void IndexHandler :: writeIndexToFile(){
    std::ofstream indexWriter;
    indexWriter.open(indexPath);
    // write the index out
    if(whatIndexToUse){
        indexWriter << avlIndex->getNumOfIndexes() << std::endl;
        avlIndex->write(indexWriter);
    }else{
        indexWriter << hashIndex->getNumOfIndexes() << std::endl;
        hashIndex->write(indexWriter);
    }
    indexWriter.close();
}

void IndexHandler :: readIndexFromFile(){

    std::vector<std::pair<std::string,int>> workingList;

    std::ifstream indexReader;
    indexReader.open(indexPath);
    if(!indexReader.eof()){
        // read from txt file & push to indexinterface
        unsigned long size;
        std::string wordTemp;
        std::string numTemp;
        std::string countTemp;
        std::string stringLine;
        std::stringstream line;
        Word* newWord;
        indexReader >> size;
        indexReader.ignore();
        for(unsigned long i = 0; i < size; ++i){
            getline(indexReader,stringLine);
            line.str(stringLine);
            line >> wordTemp;
            line >> numTemp;
            totalFrequency += stoi(numTemp);
            workingList.push_back(std::pair<std::string,int>(wordTemp,stoi(numTemp)));
            newWord = new Word(stoi(numTemp),wordTemp);
            while(line >> numTemp){
                line >> countTemp;
                std::pair<int,int> pair(stoi(numTemp),stoi(countTemp));
                newWord->pushReference(pair);
            }
            (whatIndexToUse)? avlIndex->insert(*newWord) : hashIndex->insert(*newWord);
            line.clear();
        }
    }
    indexReader.close();

    int size = workingList.size();

    int bigger;
    int biggerVal;

    for(int i = 0; i < 50; ++i){
        bigger = i;
        biggerVal = workingList[i].second;
        for(int j = i+1; j < size; ++j){
            if(workingList[j].second > biggerVal){
                bigger = j;
                biggerVal = workingList[j].second;
            }
        }
        top50Words.push_back(workingList[bigger].first);
        std::vector<std::pair<std::string,int>>::iterator k = workingList.begin();
        for(int l = 0; l < bigger; ++l) ++k;
        workingList.erase(k);
    }

    indexesLoadedFromFile = true;
}

void IndexHandler :: loadStopWords(){
    std::string stopWord;
    std::ifstream stopWordsReader;
    stopWordsReader.open("../searchEngine/stopWords.txt");
    while(stopWordsReader >> stopWord){
        stopWordsTree->insert(stopWord);
    }
    stopWordsReader.close();
}

