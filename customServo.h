/**
 * @file customServo.h
 * @brief 
 * @version 0.1
 * @date 2022-05-26
 * 
 * 
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
    short startAngle;
    short targetAngle;
    float speed;
    time_t startTime;//time at which servo started moving
    Servo servo;
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
