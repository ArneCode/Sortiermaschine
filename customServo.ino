/**
 * @file customServo.ino
 * @brief Umsetztung der CustomServo Klasse
 * 
 */
#include "customServo.h"
/**
 * @brief Setzt Variablen, die benötigt werden um den Servo zu bewegen
 * 
 */
void CustomServo::startMove() 
{
  startAngle = read();
  startTime = millis();
  done = false;
}
/**
 * @brief Bewegt den Servo mit einer vorher spezifizierten Geschwindigkeit
 * @warning Winkel überprüfen! Wenn dieser zu klein ist schlägt der Arm gegen den Stopper
 * @param newAngle 
 */
void CustomServo::write(short newAngle) 
{
  startMove();
  targetAngle = newAngle;
}
/**
 * @brief Bewegt den Servo in duration Millisekunden an den angegebenen Winkel
 * @warning Winkel überprüfen! Wenn dieser zu klein ist schlägt der Arm gegen den Stopper
 * @param newAngle 
 * @param duration 
 */
void CustomServo::write(short newAngle, time_t duration) 
{
  write(newAngle);
  speed = (float)(targetAngle - startAngle) / (float)duration;
}
/**
 * @brief Steuert den Servo direkt an, enspricht dem normalen <a href="https://www.arduino.cc/reference/en/libraries/servo/write/" target="_blank">Servo::write</a>
 * @warning Winkel überprüfen! Wenn dieser zu klein ist schlägt der Arm gegen den Stopper
 * @param angle 
 */
void CustomServo::writeDirect(short angle)
{
  Servo::write(angle);
}
/**
 * @brief Setzt eine neue Geschwindigkeit des Servos
 * @details Kann auch ausgeführt werden während der Servo sich schon bewegt
 * @param newSpeed Die neue Geschwindigkeit in Grad pro Millisekunde
 */
void CustomServo::setSpeed(float newSpeed) 
{
  startMove();
  speed = newSpeed;
}
/**
 * @brief Aktualisiert die Position des Servomotors
 * @details Wird von loop() aufgerufen
 */
void CustomServo::updatePos() 
{
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
bool CustomServo::isDone() 
{
  return read() == targetAngle;
}
/**
 * @brief Stoppt den Servo
 * 
 */
void CustomServo::stop() 
{
  done = true;
}
/**
 * @brief Lässt den Servo weiterlaufen
 * 
 */
void CustomServo::start() 
{
  done = false;
}