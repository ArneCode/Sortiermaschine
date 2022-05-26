/**
 * @file customServo.ino
 * @brief Umsetztung der CustomServo Klasse
 * 
 */
#include "customServo.h"
/**
 * @brief Setzt Variabeln, die benötigt werden um den Servo zu bewegen
 * 
 */
void CustomServo::startMove() {
  startAngle = read();
  startTime = millis();
  done = false;
}
/**
 * @brief bewegt den Servo mit einer vorher spezifizierten Geschwindigkeit
 * 
 * @param newAngle 
 */
void CustomServo::write(short newAngle) {
  startMove();
  targetAngle = newAngle;
}
/**
 * @brief bewegt den Servo in <b>duration</b> ms
 * 
 * @param newAngle 
 * @param duration 
 */
void CustomServo::write(short newAngle, time_t duration) {
  write(newAngle);
  speed = (float)(targetAngle - startAngle) / (float)duration;
}
void CustomServo::writeDirect(short angle){
  Servo::write(angle);
}
void CustomServo::setSpeed(float newSpeed) {
  startMove();
  speed = newSpeed;
}
void CustomServo::updatePos() {
  if (done) {
    return;
  }
  long timePassed = millis() - startTime;
  short newAngle;
  if (targetAngle > startAngle) {
    newAngle = startAngle + timePassed * speed;
    if (newAngle >= targetAngle) {
      Servo::write(targetAngle);
      done = true;
      return;
    }
  } else {
    newAngle = startAngle - timePassed * speed;
    if (newAngle <= targetAngle) {
      Servo::write(targetAngle);
      done = true;
      return;
    }
  }
  Servo::write(newAngle);
}
/**
 * @brief Gibt an, ob der Servo angekommen ist
 * 
 * @return true 
 * @return false 
 */
bool CustomServo::isDone() {
  return read() == targetAngle;
}
/**
 * @brief stoppt den Servo
 * 
 */
void CustomServo::stop() {
  done = true;
}
/**
 * @brief lässt den Servo weiterlaufen
 * 
 */
void CustomServo::start() {
  done = false;
}