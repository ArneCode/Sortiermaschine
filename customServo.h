/**
 * @file customServo.h
 * @brief Header Datei der CustomServo Klasse
 */
#ifndef CUSTOMSERVO_H
#define CUSTOMSERVO_H
#include <Servo.h>
#include "header.h"
/**
 * @brief Eine Eigene Servo-Klasse, die es ermöglicht den Servo mit verschiedenen Geschwindigkeiten zu bewegen
 * 
 */
class CustomServo: public Servo {
    /**
     * @brief der Winkel an dem sich der Servo bei Start der Animation befand
     * 
     */
    short startAngle;
    /**
     * @brief der Ziel Winkel
     * 
     */
    short targetAngle;
    /**
     * @brief die Geschwindigkeit des Servos in Grad pro Millisekunde
     * 
     */
    float speed;
    /**
     * @brief Zeitpunkt an dem der Servo anfing sich zu bewegen (in Millisekunden)
     * 
     */
    time_t startTime;//time at which servo started moving
    void startMove();
  public:
    bool done;
    void write(short newAngle);
    void write(short newAngle, time_t duration);
    void writeDirect(short angle);
    void setSpeed(float newSpeed);
    void updatePos();
    void stop();
    void start();
    bool isDone();
};
#endif
