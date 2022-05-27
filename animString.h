/**
 * @file animString.h
 * @brief Header datei für eine Mehrzahl von animierbaren Strings und der Callable Klasse
 * 
 */
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

/**
 * @brief Ein Call der vom CallHandler aufgerufen werden kann.
 * @details Wird nie selbst instanziert sondern nur Abgeleitete Klassen
 * 
 */
class Callable {
  virtual void run() {} //virtual->must be implemented by derived classes
  virtual bool isDone() {}
  virtual ~Callable() {} //let's derived classes free their own memory. ~functions are called when the object is deleted
};
/**
 * @brief Ein Call der eine Funktion ausführt
 * 
 */
struct FuncCall: public Callable {
    /**
     * @brief die Funktion die aufgerufen wird, wenn der Call an der Reihe ist
     * 
     */
    func_t<void> call;
    /**
     * @brief bestimmt, ob dieser Call vorbei ist
     * 
     */
    func_t<bool> _isDone;
    FuncCall(func_t<void> call, func_t<bool> _isDone):
      call(call), _isDone(_isDone) {  }
    FuncCall(func_t<void> call): //when no isDone function is provided, isDone defaults to true
      call(call), _isDone([](){return true;}) {}
    virtual ~FuncCall() {}
    void run();
    bool isDone();
};
/**
 * @brief Ein String der auf dem AnimatableLcd angezeigt werden kann
 * @details Animationen werden von dieser Klasse abgeleitet
 */
struct LcdString: public Callable {
  /**
   * @brief der Text der angezeigt wird
   * 
   */
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
/**
 * @brief Die Klasse für animierbare LcdStrings
 * @details Wird nie selbst instanziert aber Lcd Animationen erben von dieser Klasse
 */
class AnimString: public LcdString {
  protected:
   /**
   * @brief wie lange ein Schritt der Animation dauert
   * 
   */
    time_t stepDuration;
    /**
     * @brief wann die animation begann
     * 
     */
    time_t animStart;
    /**
     * @brief wann das letzte mal die Anzeige erneuert wurde
     * 
     */
    time_t lastRefresh;
  public:
    using LcdString::LcdString;
    virtual ~AnimString() {}
    virtual void init() {}
    void run();
};
/**
 * @brief Die Klasse der Lcd Ladeanimationen
 * @details Zeigt Acht Ladebalken und den Fortschritt in Prozent an
 */
class LcdLoadingAnim: public AnimString {
  public:
    using AnimString::AnimString;
    void  init();
    void update();
};
/**
 * @brief Die Klasse der Lcd Punktanimationen
 * @details zeigt immer wieder keinen, dann einen, dann zwei, dann drei und letzendlich wieder keinen Punkt nach dem Text an
 */
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