/**
 * @file callHandler.h
 * @brief header datei für den CallHandler
 */
#ifndef CALLHANDLER_H
#define CALLHANDLER_H
class CallHandler;

#include "header.h"
#include "animString.h"
/**
 * 
 * @brief Klasse, die Calls nacheinander aufruft
 * @details Ermöglicht es Calls wie z.B. Funktionen nacheinander aufzurufen, ohne die delay() Funktion zu verwenden
 * 
 */
class CallHandler {//calls functions after a certain delay
    /**
     * @brief Die liste der aktuellen Calls
     * @details Wird mithilfe von CallHandler::setCalls gesetzt.
     * @note Die Calls werden im <a href="https://www.geeksforgeeks.org/stack-vs-heap-memory-allocation/" target="_blank">Heap</a> gespeichert, das heißt zum einen, dass sie zwischen Funktionen hin- und hergegeben werden können, zum anderen aber auch, dass sie mithilfe von CallHandler::deleteCalls manuell wieder gelöscht werden müssen
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
  /**
   * @brief Gibt an, ob der CallHandler fertig ist
   * 
   */
    bool running = false;
    /**
     * @brief Gibt an, ob der Servo pausiert ist, wird zu true gesetzt, wenn der Stoppknopf gedrückt wird
     * 
     */
    bool paused = false;
    void deleteCalls();
    void setCalls(Callable* newCallPtrs[], size_t nCalls);
    void update();
};
#endif