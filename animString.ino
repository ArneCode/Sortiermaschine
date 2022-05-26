#include "animLcd.h"
#include "callHandler.h"
#include "header.h"
#include "animString.h"

void FuncCall::run() {
  call();
}
bool FuncCall::isDone() {
  return _isDone();
}
void LcdString::run() {
  callStart = millis();
  lcd->doAnimation = false;
  lcd->printPretty(this->text);
}
bool LcdString::isDone() {
  return millis() - callStart > duration;
}
void AnimString::run() {
  callStart = millis();
  lcd->clear();
  lcd->setAnimation(this);
  animStart = millis();
  lastRefresh = millis();
  init();
}
void  LcdLoadingAnim::init() {
  stepDuration = duration / 9;
  if (text.length() > 16) {
    Serial.print("warning: text given for loading animation is to long, text: ");
    Serial.println(text);
  }
  lcd->printCentered(text);
  lcd->setCursor(LOADING_BAR_OFFSET, 1);
  for (int i = 0; i < 8; i++) {
    lcd->write(0);
  }
  lcd->print("0% ");
}
void LcdLoadingAnim::update() {
  time_t time = millis();
  short percent = (time - animStart) * 100 / duration;
  if (time - lastRefresh > stepDuration) {
    short nToFill = percent * 9 / 100;
    if (nToFill == 0) {
      return;
    }
    lcd->setCursor(nToFill + LOADING_BAR_OFFSET - 1, 1);
    lcd->write(1);
    lastRefresh = time;
  }
  lcd->setCursor(8 + LOADING_BAR_OFFSET, 1);
  lcd->print(percent);
  lcd->print("%");
}
void LcdDotAnim::init() {
  lcd->printPretty(text + "\1\1\1");//spaces that can't be broken up to newlines
}
void LcdDotAnim::update() {
  time_t time = millis();
  if ((time - lastRefresh) < stepDuration) {
    return;
  }
  lastRefresh = time;
  int numDots = ((time - animStart) / stepDuration) % 4;
  char dots[4];
  for (int i = 0; i < 3; i++) {
    if (i < numDots) {
      dots[i] = '.';
    } else {
      dots[i] = '\1';
    }
  }
  dots[3] = '\0';
  lcd->printPretty(text + dots);
}
