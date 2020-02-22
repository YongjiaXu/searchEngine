//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_QUERYENGINE_H
#define INC_20S_3353_PA01_QUERYENGINE_H


#include "query.h"
#include "opinion.h"
#include <sstream>
#include <list>

class QueryEngine{

private:

    void inputAnalysisHelper(std::stringstream&, Query*&);\

public:

    QueryEngine();

    Query& parseInput(std::string);

    void rankResult(std::vector<Opinion>&);

};


#endif //INC_20S_3353_PA01_QUERYENGINE_H
