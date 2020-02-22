#include "searchengine.h"

SearchEngine::SearchEngine(){
    iH = new IndexHandler;
    dP = new DocumentParser;
    iH->setDP(*dP);
    dP->setiH(*iH);
    mode = 'I';
}

SearchEngine :: ~SearchEngine(){
    delete iH;
    delete dP;
}


//**************************Functions called by run**************************

bool SearchEngine :: prompt(){
    char option;

    std::cout << "Make your selection here: ";

    std::cin >> option;
    std::cin.ignore();
    option = tolower(option);

    /* reference:
     * 'Q' for new query
     * 'A' for load index in AVL tree
     * 'H' for load index in Hash Table
     * 'M' for switch to maintenance mode
     * 'N' for input new opinions folder path
     * 'C' for clearing index
     * 'I' for switch to Interactive mode
     * 'E' for exit program
     * and there are all lowercase for logistic
     */

    if(option == 'e'){
        std::cout << "You have pressed 'E' \nThank you for using our search egine!" << std::endl;
        return false;
    }

    if(mode == 'I'){

        switch(option){

            case 'q':
                std::cout << "You have pressed 'Q' \nEnter your query here: ";
                processNewQuery();
                std::cout << "Taking you back to menu..." << std::endl;
                sleep(2);
                break;

            case 's':
                std::cout << "You have pressed 'S'";
                printStats();
                std::cout << "Press anything to continue" << std::endl;
                std::cin.ignore();
                break;


            case 'a':
                clearScreen();
                std::cout << "You have pressed 'A' \nLoading index into AVL tree... " << std::endl;
                loadIndex(true);
                std::cout << "Finished! Taking you back to menu" << std::endl;
                sleep(2);
                break;

            case 'h':
                clearScreen();
                std::cout << "You have pressed 'H' \nLoading index into Hash table... " << std::endl;
                loadIndex(false);
                std::cout << "Finished! Taking you back to menu..." << std::endl;
                sleep(2);
                break;

            case 'm':
                std::cout << "You have pressed 'M' \nTaking you to Maintanence menu..." << std::endl;
                mode = 'M';
                sleep();
                break;

            default:
                std::cout << "No such option, try again" << std::endl;
                sleep();
        }

    }else{

        switch(option){

            case 'n':
                std::cout << "You have pressed 'N' \nEnter your path here: ";
                addNewPath();
                std::cout << "Refreshing menu..." << std::endl;
                sleep(2);
                break;

            case 'c':
                std::cout << "Clearing all indexes..." << std::endl;
                clearIndex();
                std::cout << "Finsihed! Refreshing menu..." << std::endl;
                sleep();
                break;

            case 'p':
                std::cout << "Populating indexes..." << std::endl;
                populateIndex();
                std::cout << "Finsihed! Refreshing menu..." << std::endl;
                sleep();
                break;

            case 'i':
                std::cout << "You have pressed 'I' \nTaking you to Interactive menu..." << std::endl;
                mode = 'I';
                sleep();
                break;

            default:
                std::cout << "No such option, try again" << std::endl;
                sleep();
        }
    }

    return true;
}

void SearchEngine :: displayMenu(){
    clearScreen();

    // printing the logo
    printFromFile("../searchEngine/GUI/logo.txt");

    if(mode == 'I'){
        printFromFile("../searchEngine/GUI/InteractiveMode.txt");
    }else{
        printFromFile("../searchEngine/GUI/MaintenanceMode.txt");
    }
}


//************************Functions call to other classes**************************

void SearchEngine :: addNewPath(){
    std::string newPath;
    getline(std::cin,newPath);
    dP->addNewPath(newPath);
}

void SearchEngine :: printStats(){
    std::cout << "Total number of opinions indexed: " << iH->getNumberOfOpinionsIndexed() << std::endl;
    std::cout << "Average number of words indexed per opinons: " << iH->getAverageWordsPerOpinion() << std::endl;
    std::cout << "Heres Top 50 of most frequent word: " << std::endl;
    std::vector<std::string> top50 = iH->getTop50Words();
    for(int i = 0; i < top50.size(); ++i){
        std::cout << i+1 << " " << top50[i] << std::endl;
    }
}

void SearchEngine :: clearIndex(){
    iH->clearIndex();
    dP->resetCurrentPath();
}

void SearchEngine :: populateIndex(){
    if(dP->hasNewPath()){
        //load into hashtable
        dP->startParsing();
        iH->writeIndexToFile();
        iH->clearIndex(); // clear indexes
    }else{
        std::cout << "Nothing new to be populated" << std::endl;
    }
}

void SearchEngine :: loadIndex(bool isAVL){
    iH->setWhatIndexToUse(isAVL);
    iH->startGeneratingIndexes();
}

void SearchEngine :: processNewQuery(){
    std::string newQuery;
    getline(std::cin,newQuery);
    if(iH->getIndexesLoaded()){
        qE.rankResult(iH->search(qE.parseInput(newQuery)));
    }else{
        std::cout << "Indexes have not been loaded yet, please load indexes first" << std::endl;
    }
}

//**************************GUI Helper Funcs**************************

void SearchEngine :: sleep(int time){
    std::chrono::seconds sleepTime(time);
    std::this_thread::sleep_for(sleepTime);
}

void SearchEngine :: clearScreen(int skips){
    std::cout << std::string(skips,'\n');
}

void SearchEngine :: printFromFile(std::string path){
    char buffer[100];
    std::ifstream input;
    input.open(path);
    if(!input){std::cout << "fail to access image" << std::endl;input.close();}
    else{
        while(true){
            input.getline(buffer,100);
            if(input.eof()){break;}
            std::cout << buffer << std::endl;
        }
        input.close();
    }
}

//**************************Run Functinos**************************

void SearchEngine :: run(){
    do{displayMenu();}while(prompt()); // display menu till user exist
}

void SearchEngine :: run(std::string& path, std::string& searchWord){
    dP->directParse(path,searchWord);
}
