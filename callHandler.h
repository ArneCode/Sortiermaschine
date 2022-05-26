/**
 * @file callHandler.h
 * @author your name (you@domain.com)
 * @brief header datei für den CallHandler
 * @version 0.1
 * @date 2022-05-26
 */
#ifndef CALLHANDLER_H
#define CALLHANDLER_H
class CallHandler;

#include "header.h"
#include "animString.h"
/**
 * @brief Ermöglicht es Calls wie z.B. Funktionen nacheinander aufzurufen, ohne die delay() Funktion zu verwenden
 * 
 */
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