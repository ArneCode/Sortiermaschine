//header file for the call handler which handels calls
#ifndef CALLHANDLER_H
#define CALLHANDLER_H
class CallHandler;

#include "header.h"
#include "animString.h"

class CallHandler {//calls functions after a certain delay
    Callable** callPtrs;
    Callable** currCallPtr;
    Callable** lastCallPtr;
    time_t lastCallT;
    bool callsSet = false;
  public:
    void deleteCalls();
    bool running = false;
    void setCalls(Callable* newCallPtrs[], size_t nCalls);
    virtual ~ CallHandler();
    void update();
};
#endif