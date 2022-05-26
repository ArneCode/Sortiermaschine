/**
 * @file callHandler.ino
 * @author Arne de Borman
 * @brief Umsetzung der CallHandler Klasse
 */
#include "header.h"
#include "animString.h"
#include "callHandler.h"
/**
 * @brief setzt den Speicherplatz der von den Calls besetzt wurde frei
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
 * @warning nCalls darf auf keinen Fall größer als die tatsächliche Anzahl an Calls sein, sonst stürzt das Programm ab
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
 * @brief wechselt zum nächsten Call, wenn der Aktuelle vorbei ist und aktualisiert den jetzigen (z.B. animationen)
 * @details wird von loop aufgerufen
 */
void CallHandler::update() 
{
  if (!running) {
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
/**
 * @brief Destroy the Call Handler:: Call Handler object
 * 
 */
CallHandler::~CallHandler() {
  deleteCalls();
}
