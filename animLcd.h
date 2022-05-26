//the header file for the animatable lcd
#ifndef ANIMLCD_H
#define ANIMLCD_H
#include <LiquidCrystal_I2C.h>
class AnimatableLcd;
#include "animString.h"

const int LOADING_BAR_OFFSET = 2;
class AnimatableLcd: public LiquidCrystal_I2C {
    AnimString* animString;
  public:
    bool doAnimation = false;
    using LiquidCrystal_I2C::LiquidCrystal_I2C; //using the LiquidCrystal constructor
    void setAnimation(AnimString* _animString);
    void printCentered(String text, int length = -1, int row = 0);
    void printPretty(String text);
    void update();
    void init();
    using LiquidCrystal_I2C::print;
    void print(const String& text);
};
#endif