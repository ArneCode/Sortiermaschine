//the implementations for the animatable string class
//animatable strings are strings that can be passed to the animatable lcd
#ifndef ANIMSTRING_H
#define ANIMSTRING_H
struct Callable;
struct FuncCallable;
struct LcdString;
class LcdLoadingAnim;
class LcdDotAnim;

#include "animLcd.h"
#include "header.h"

//Callable is an object that can be called in the callhandler
struct Callable {
  virtual void run() {} //virtual->must be implemented by derived classes
  virtual bool isDone() {}
  virtual ~Callable() {} //let's derived classes free their own memory. ~functions are called when the object is deleted
};
struct FuncCall: public Callable {
    func_t<void> call;
    func_t<bool> _isDone;
    FuncCall(func_t<void> call, func_t<bool> _isDone):
      call(call), _isDone(_isDone) {  }
    FuncCall(func_t<void> call): //when no isDone function is provided, isDone defaults to true
      call(call), _isDone([](){return true;}) {}
    virtual ~FuncCall() {}
    void run();
    bool isDone();
};

struct LcdString: public Callable {
  String text;
  AnimatableLcd* lcd;
  time_t duration;
  time_t callStart;//time at which the string was written to the LCD
  LcdString(String text, AnimatableLcd* lcd, time_t duration = 0)
    : text(text), duration(duration), lcd(lcd) {  }
  virtual ~LcdString() {}
  bool isDone();
  virtual void run();
  virtual void update() {}
};
class AnimString: public LcdString {
  protected:
    time_t stepDuration;
    time_t animStart;
    time_t lastRefresh;
  public:
    using LcdString::LcdString;
    virtual ~AnimString() {}
    virtual void init() {}
    void run();
};
class LcdLoadingAnim: public AnimString {
  public:
    using AnimString::AnimString;
    void  init();
    void update();
};
class LcdDotAnim: public AnimString {
  public:
    LcdDotAnim(String text, AnimatableLcd* lcd, time_t duration = 0, time_t _stepDuration = 500)
      : AnimString(text, lcd, duration) {
      stepDuration = _stepDuration;
    }
    void init();
    void update();
};
#endif