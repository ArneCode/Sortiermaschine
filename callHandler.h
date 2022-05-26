/**
 * @file callHandler.h
 * @author your name (you@domain.com)
 * @brief header datei für den CallHandler
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
    /**
     * @brief Die liste der aktuellen Calls
     * @details Wird mithilfe von CallHandler::setCalls gesetzt.
     Die Calls werden im Heap gespeichert, das heißt zum einen, dass sie zwischen Funktionen hin- und hergegeben werden können, zum anderen aber auch, dass sie mithilfe von CallHandler::deleteCalls manuell wieder gelöscht werden müssen
     */
    Callable** callPtrs;
    /**
     * @brief Der Call der zurzeit ausgeführt wird
     * @details Es handelt sich hierbei um einen Pointer-Pointer. 
     Der Pointer zeigt zu einer Stelle in der CallHandler::callPtrs Liste, die wiederum zum tatsächlichen Call zeigt
     */
    Callable** currCallPtr;
    /**
     * @brief Der letzte Call
     * @details Wird benutzt um zu wissen, wann der letzte Call ausgeführt wurde
     */
    Callable** lastCallPtr;
    /**
     * @brief Der Zeitpunkt an dem der letzte Call ausgeführt wurde
     * 
     */
    time_t lastCallT;
    /**
     * @brief Sagt aus, ob CallHandler::callPtrs zu einer gültigen Speicheradresse zeigt 
     * 
     */
    bool callsSet = false;
  public:
    void deleteCalls();
    bool running = false;
    void setCalls(Callable* newCallPtrs[], size_t nCalls);
    virtual ~ CallHandler();
    void update();
};
#endif