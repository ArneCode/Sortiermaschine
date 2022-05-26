#include "header.h"
#include "animString.h"
#include "callHandler.h"
void CallHandler::deleteCalls() {
  if (!callsSet) {
    return;
  }
  callsSet = false;
  for (Callable** callPtr = callPtrs; callPtr <= lastCallPtr; callPtr++) {
    delete *callPtr;
  }
  delete callPtrs;
}
void CallHandler::setCalls(Callable* newCallPtrs[], size_t nCalls) {
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

void CallHandler::update() {
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
CallHandler::~CallHandler() {
  deleteCalls();
}
