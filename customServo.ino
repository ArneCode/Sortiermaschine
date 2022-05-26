#include "customServo.h"
void CustomServo::startMove() {
  startAngle = read();
  startTime = millis();
  done = false;
}
void CustomServo::write(short newAngle) {
  startMove();
  targetAngle = newAngle;
}
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
bool CustomServo::isDone() {
  return read() == targetAngle;
}
void CustomServo::stop() {
  done = true;
}
void CustomServo::start() {
  done = false;
}