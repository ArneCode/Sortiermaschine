#include <LiquidCrystal_I2C.h>
#include "header.h"
#include "animLcd.h"
#include "callHandler.h"

const byte loading_empty_c[8] = { //is used to define a custom character representing a square
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
const byte loading_full_c[8] = { //is used to define a custom character representing a filled square
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void AnimatableLcd::init() {
  LiquidCrystal_I2C::init();
  backlight();
  noCursor();
  lcd.createChar(0, loading_empty_c);
  lcd.createChar(1, loading_full_c);
}
void AnimatableLcd::setAnimation(AnimString* _animString) {
  doAnimation = true;
  animString = _animString;
}
void AnimatableLcd::print(const String& text){
  //custom print with ability to use custom characters, just inserst the number of the custom character in the string (\1n for the nth character)
  //and it will be converted to the custom character (\1n so that \0 doesn't appear in the string, because it means end of string)
  for(char c:text){
    if(c>=8&&c<=15){//if it is a custom character
      write(c-8);
    }else if(c==1){//defining a non-newline space
      LiquidCrystal_I2C::print(" ");
    }
    else{
      LiquidCrystal_I2C::print(c);
    }
  }
}
void AnimatableLcd::printCentered(String text, int length = -1, int row = 0) { //length<=16
  if (length == -1) {
    length = text.length();
  }
  int offset = (16 - length) / 2; //rundet immer ab, da int
  setCursor(offset, row);
  print(text);
}
void AnimatableLcd::printPretty(String text) { //handelt zeilenumbrüche und schreibt zentriert
  clear();
  //Serial.print("printing pretty: ");
  //Serial.println(text);
  int length = text.length();
  if (length <= 16) {
    printCentered(text, length);
    return 0;
  }
  int spacePos = -1;
  for (int i = 15; i >= 0; i--) {
    if (text[i] == ' ') {
      spacePos = i;
      break;
    }
  }
  String row1, row2;
  if (spacePos != -1) {
    row1 = text.substring(0, spacePos);
    row2 = text.substring(spacePos + 1);
  } else {
    row1 = text.substring(0, 16);
    row2 = text.substring(16);
  }
  printCentered(row1, row1.length(), 0);
  printCentered(row2, row2.length(), 1);
}
void AnimatableLcd::update() {
  if (!doAnimation) {
    return;
  }
  animString->update();
}