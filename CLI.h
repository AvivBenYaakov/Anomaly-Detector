#ifndef CLI_H_
#define CLI_H_

#include <cstring>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO* dio;
    Command** commands{};
    // you can add data members
public:
    explicit CLI(DefaultIO* dio);
    void start();
    void printMenu();
    void getCommands();
    virtual ~CLI() ;
};

#endif /* CLI_H_ */