/**
 * @file callHandler.ino
 * @brief Umsetzung der CallHandler Klasse
 */
#include "header.h"
#include "animString.h"
#include "callHandler.h"
/**
 * @brief setzt den Speicherplatz der von den Calls besetzt wurde mithilfe von delete frei
 * 
 */
void CallHandler::deleteCalls() 
{
  if (!callsSet) {
    return;
  }
  callsSet = false;
  for (Callable** callPtr = callPtrs; callPtr <= lastCallPtr; callPtr++) {
    delete *callPtr;
  }
  delete callPtrs;
}
/**
 * @brief Setzt die neuen Calls, die ausgeführt werden sollen
 * @note Calls werden im <a href="https://www.geeksforgeeks.org/stack-vs-heap-memory-allocation/" target="_blank">Heap</a> gespeichert, um sie zwischen Funktionen hin- und hergeben zu können und sie zu benutzen nachdem die Exekution abgeschlossen ist (bzw an das CallHandler Objekt). Sie müssen aber such manuell mithilfe von CallHandler::deleteCalls gelöscht werden
 * @warning nCalls darf auf keinen Fall größer als die tatsächliche Anzahl an Calls sein, sonst stürzt das Programm ab, weil es versucht nicht vorhandene Calls auszuführen
 * @param newCallPtrs 
 * @param nCalls 
 */
void CallHandler::setCalls(Callable* newCallPtrs[], size_t nCalls) 
{
  /*if(callsSet){ //doing this would result in two sets of calls being in heap at once
    deleteCalls(); //solution is to delete previus calls before initializing a new one
  }*/
  callPtrs = newCallPtrs;
  callsSet = true;
  currCallPtr = callPtrs;
  (*currCallPtr)->run();
  lastCallPtr = callPtrs + nCalls - 1;
  lastCallT = millis();
  running = true;
}
/**
 * @brief Wechselt zum nächsten Call, wenn der Aktuelle vorbei ist und aktualisiert den jetzigen (z.B. animationen)
 * @details Wird von loop() aufgerufen
 */
void CallHandler::update() 
{
  if (!running||paused) {
    return;
  }
  time_t timePassed = millis() - lastCallT;
  if ((*currCallPtr)->isDone()) {//->currCall->isDone()
    if (currCallPtr == lastCallPtr) {
      running = false;
      return;
    }
    currCallPtr++;
    (*currCallPtr)->run();//->currCall->run();
  }
}