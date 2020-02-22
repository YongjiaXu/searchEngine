#include "documentparser.h"
#include "indexhandler.h"

DocumentParser::DocumentParser()
{
    iH = nullptr;
    currentPath = 0;
    fileNum = 0;
    pathsPath = "paths.txt";
    readPathsFromFile();
}

DocumentParser :: ~DocumentParser(){
    storePathsToFile();
}

//***************************Checker Functions**************************

bool DocumentParser :: validChar(const char &c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool DocumentParser :: validNum(const char &c){
    return (c >= '0' && c <= '9');
}

bool DocumentParser :: findIfEmpty(const std::string& text){
    if(text.find_last_of('"') - text.find_first_of('"') > 1) return false;
    else return true;
}

//***************************I/O Functions**************************

void DocumentParser :: storePathsToFile(){
    std::ofstream pathWriter;
    pathWriter.open(pathsPath);
    pathWriter << currentPath << std::endl;
    for(int i = 0; i < paths.size(); ++i){
        pathWriter << paths[i] << std::endl;
    }
    pathWriter.close();
}

void DocumentParser :: readPathsFromFile(){
    std::ifstream pathReader;
    pathReader.open(pathsPath);
    std::string temp;
    if(pathReader.good()){
        pathReader >> currentPath;
        while(pathReader >> temp){
            paths.push_back(temp);
        }
    }
    pathReader.close();
}
//***************************Info extracting Functions**************************

void DocumentParser :: timeExtraction(std::string &s){
    s = s.substr(18);
    int startPos = s.find_first_of('"')+1;
    s = s.substr(startPos,s.find_first_of('T')-startPos);
}

void DocumentParser :: htmlAnalysis(Opinion & op, std::string & text){

    int endPos;
    int startPos;
    int fisrtPos;

    std::string partyText;
    std::string partyName;
    std::string judgeName;
    std::stringstream ss;

    if(text.find("<p class=\\\"parties\\\">") != std::string::npos){

        startPos = text.find("<p class=\\\"parties\\\">");
        endPos = text.find("</p>",startPos);
        partyText = text.substr(startPos,endPos-startPos);

        while(partyText.find_first_of('<') != std::string::npos){
            fisrtPos = partyText.find_first_of('<');
            partyText.replace(fisrtPos,partyText.find_first_of('>',fisrtPos)-fisrtPos+1," ");
        }

        while(partyText.find("v.") != std::string::npos){
            partyText.replace(partyText.find_first_of("v."),2,"\n");
        }

        ss.str(partyText.c_str());
        while(std::getline(ss,partyName)){
            partyName = partyName.substr(0,partyName.find_first_of(','));
            op.pushNewParty(partyName);
        }

    }else{
        op.pushNewParty(std::string("see text"));
    }
}

std::string DocumentParser :: findJudge(std::string & text){
    std::string judgeName;
    std::stringstream ss;
    if(text.find("Mr. Justice") != std::string::npos){

        ss.str(text.substr(text.find("Mr. Justice")+12).c_str());
        ss >> judgeName;
        for(int i = 0; i < judgeName.size(); ++i) if(!validChar(judgeName[i])) judgeName.replace(i,1," ");

    }else{

        judgeName = "see text";

    }
    return judgeName;
}

std::string DocumentParser :: first300WordsExtraction(std::string& s, bool isHtml, int& wordCounter){

    char c;
    std::string temp;
    std::string finishedText;
    std::stringstream ss;

    if(!isHtml){
        for(std::string::iterator i = s.begin(); i != s.end(); ++i){

            c = *i;

            if(validChar(c) || validNum(c)) temp.push_back(c);
            else{
                if(temp.size() > 0){
                    ++wordCounter;
                    if(wordCounter < 300) ss << temp;
                }
                temp.clear();
                if(wordCounter < 300){
                    if(c == '\\'){
                        ++i;
                        ss << ' ';
                    }
                    else if(c == ' '){
                        ss << ' ';
                        while(*i == ' ') ++i;
                        --i;
                    }
                    else if(c == ',') ss << c;
                    else if(c == '-') ss << c;
                    else if(c == '\'') ss << c;
                    else if(c == '.') ss << c;
                }
            }
        }
    }else{
        for(std::string::iterator i = s.begin(); i != s.end(); ++i){

            c = *i;

            if(validChar(c) || validNum(c)) temp.push_back(c);
            else{
                if(temp.size() > 0){
                    ++wordCounter;
                    if(wordCounter < 300) ss << temp;
                }
                temp.clear();
                if(wordCounter < 300){
                    if(c == '<'){
                        while(*i != '>') ++i;
                        ss << ' ';
                    }
                    else if(c == '\\'){
                        ++i;
                        ss << ' ';
                    }
                    else if(c == ' '){
                        ss << ' ';
                        while(*i == ' ') ++i;
                        --i;
                    }
                    else if(c == ',') ss << c;
                    else if(c == '-') ss << c;
                    else if(c == '\'') ss << c;
                    else if(c == '.') ss << c;
                }
            }
        }
    }

    ss << "....." << std::endl;
    finishedText = ss.str();
    return finishedText;
}

//***************************Parsing Functions**************************

int DocumentParser :: stripFileID(const std::string& path){
    size_t start = path.find_last_of('/')+1;
    return stoi(path.substr(start,path.find_last_of('.')-start));
}

void DocumentParser :: trimAndStem(std::string & word){
    Porter2Stemmer::trim(word);
    size_t wordSize = word.size();
    if(wordSize > 3){
        if(!iH->isStopWords(word)){
            Porter2Stemmer::stem(word);
            if(!(word == "" || word.size() < 3)){
                iH->loadWord(word);
            }
        }
    }
}

// FIXMEEEE need to add word frequency into Word

void DocumentParser :: parseThisPath(std::string & path){
    int fileID;
    std::string temp;
    std::ifstream jsonReader;
    for(const auto& entry : fs::directory_iterator(path)){
        ++fileNum;
        fileID = stripFileID(entry.path());
        iH->setWorkingOpinion(fileID);
        jsonReader.open(entry.path());

        for(int i = 0; i < 16; ++i){getline(jsonReader,temp);} // jump to the gold mine
        while(getline(jsonReader,temp)){
            if(temp.substr(0,20).find("\"plain_text\":") != std::string::npos){
                temp = temp.substr(15);
                if(temp.find("Certiorari denied") == std::string::npos || temp.find("Petition for rehearing denied") == std::string::npos){
                    if(!findIfEmpty(temp)){textParsing(temp,false);break;}
                }
            }
            else if(temp.substr(0,20).find("html") != std::string::npos){
                temp = temp.substr((9));
                if(temp.find("Certiorari denied") == std::string::npos || temp.find("Petition for rehearing denied") == std::string::npos){
                    if(!findIfEmpty(temp)){textParsing(temp,true);break;}
                }
            }
        }

        jsonReader.close();
    }
}

void DocumentParser :: textParsing(std::string& text, bool isHtml){
    int counter = 0;
    char c;
    char buffer[15] = "";
    std::string word;

    if(isHtml){
        for(std::string::iterator i = text.begin(); i != text.end(); ++i){
            c = *i;
            if(!validChar(c)){ // this is when a word is finished
                if(c == '\\'){++i;}
                if(c == '<'){while(*i != '>'){++i;}}
                if(counter > 0){
                    if(counter > 3){ //if valid size
                        word = buffer;
                        trimAndStem(word);
                    }
                    counter = 0;
                    memset(buffer,0,15);
                }
            }else{
                buffer[counter] = c;
                ++counter;
                if(counter == 15){ // if buffer is full clean buffer
                    counter = 0;
                    memset(buffer,0,15);
                    while(validChar(*i)){ // cut out the second part of word if there is any
                        ++i;
                    }
                }
            }
        }
    }else{
        for(std::string::iterator i = text.begin(); i != text.end(); ++i){
            c = *i;
            if(!validChar(c)){ // this is when a word is finished
                if(c == '\\'){++i;}
                if(counter > 0){
                    if(counter > 3){ //if valid size
                        word = buffer;
                        trimAndStem(word);
                    }
                    counter = 0;
                    memset(buffer,0,15);
                }
            }else{
                buffer[counter] = c;
                ++counter;
                if(counter == 15){ // if buffer is full clean buffer
                    counter = 0;
                    memset(buffer,0,15);
                    while(validChar(*i)){ // cut out the second part of word if there is any
                        ++i;
                    }
                }
            }
        }
    }
}

//***************************Getter Function**************************

int DocumentParser :: getFileNum(){
    if(fileNum != 0) return fileNum;
    else{
        for (int i = 0; i < paths.size(); ++i) {
            for(const auto& entry : fs::directory_iterator(paths[i])){
                ++fileNum;
            }
        }
        return fileNum;
    }
}

bool DocumentParser :: hasNewPath(){
    return (currentPath < paths.size());
}

//***************************Setter Functions**************************

void DocumentParser :: setiH(IndexHandler &iH){
    this->iH = &iH;
}

void DocumentParser :: resetCurrentPath(){
    currentPath = 0;
}

//***************************Parsing Starters**************************

void DocumentParser :: startParsing(){
    // gets call when populate index
    if(paths.size() == 0){
        std::cout << "No paths found\nenter a new path from maintence mode and try again" << std::endl;
    }else{
        int size = paths.size();
        if(currentPath < size-1){  // meaning not all paths have already parsed
            for(int i = currentPath; i < size; ++i){
                // parse all reamining paths
                parseThisPath(paths[i]);
            }
            currentPath = size;
        }
    }
}

void DocumentParser :: directParse(std::string &path, std::string &searchWord){

    std::string printWord(searchWord);

    Porter2Stemmer::stem(searchWord);

    parseThisPath(path);

    std::cout << "Total of " << iH->getNumberOfOpinionsIndexed() << " words being indexed" << std::endl;

    Word findThisWord(searchWord);
    Word foundWord = iH->search(findThisWord);
    if(!(findThisWord == foundWord)){
        std::cout << "Word " << printWord << " does not exists in this dataset" << std::endl;
    }else{
        std::cout << "Total of appearance for " << printWord << " : " << foundWord.getToTalAppearance() << std::endl;
        std::cout << "Total of opinions contain " << printWord << " : " << foundWord.getReference().size() << std::endl;
        std::cout << "Total files parsed: " << fileNum << std::endl;
    }
    //iH->print();
}

//***************************Function called by SearchEngine**************************

void DocumentParser :: addNewPath(std::string newPath){
    paths.push_back(newPath);
}

//***************************Parsing Opinion Functions**************************

std::string DocumentParser :: searchOpinion(int id){
    bool fileFound = false;
    std::string filePath;
    for(int i = 0; i < paths.size(); ++i){ // iterate all paths
        for(const auto& entry : fs::directory_iterator(paths[i])){
            if(id == stripFileID(entry.path())){
                filePath = entry.path();
                fileFound = true;
                break;
            }
        }
        if(fileFound) break;
    }
    return filePath;
}

Opinion& DocumentParser :: parseOpinion(std::string filePath,  Query& requestInfo, std::pair<int,int>& reference, double factor){

    int wordCounter = 0;

    bool firstPartDone = false;
    bool timeExtracted = false;

    std::string temp;

    std::ifstream reader;

    Opinion* opinionPtr = new Opinion();

    reader.open(filePath);

    // parsing magic here
    while(getline(reader,temp)){

        // get time first
        if(!timeExtracted){

            if(temp.find("date_created") != std::string::npos){
                timeExtraction(temp);
                opinionPtr->setCreatedDate(temp);
                continue;
            }else if(temp.find("date_modified") != std::string::npos){
                timeExtraction(temp);
                opinionPtr->setModifiedDate(temp);
                timeExtracted = true;
                continue;
            }

        }else{

            if(temp.find("\"plain_text\":") != std::string::npos){
                temp = temp.substr(16);
                if(!findIfEmpty(temp)){
                    temp = temp.substr(1);
                    opinionPtr->setJudge(findJudge(temp));
                    opinionPtr->setFirst300Words(first300WordsExtraction(temp,false,wordCounter));
                    firstPartDone = true;
                    continue;
                }
            }else if(temp.find("\"html") != std::string::npos){
                temp = temp.substr(9);
                if(!findIfEmpty(temp)){
                    temp = temp.substr(temp.find_first_of('\"',temp.find_first_of('\"',temp.find_first_of('\"')))+1);
                    if(!firstPartDone){
                        opinionPtr->setJudge(findJudge(temp));
                        opinionPtr->setFirst300Words(first300WordsExtraction(temp,true,wordCounter));
                    }
                    htmlAnalysis(*opinionPtr,temp);
                    break;
                }
            }
        }
    }

    opinionPtr->setRelavency((reference.second/wordCounter) * factor);

    // after assign all opinion data temp varaibles & counted all freqencies
    return *opinionPtr;
}

std::vector<Opinion>& DocumentParser :: findOpinions(std::vector<std::pair<int,int>>& references, Query& requestInfo){  // gets called from search in IndexHandler

    Opinion* opinionTemp;
    std::string opinionFilePath;
    std::vector<Opinion>* opinionlist = new std::vector<Opinion>;// this is a ptr

    if(references.size() != 0)
        for(unsigned long i = 0; i < references.size(); ++i){
            opinionFilePath = searchOpinion(references[i].first);
            opinionTemp = &parseOpinion(opinionFilePath,requestInfo,references[i],log10(fileNum/references.size()));
            (*opinionlist).push_back(*opinionTemp);
        }

    return *opinionlist;
}













