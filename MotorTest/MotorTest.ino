/*
 * Microstepping demo
 *
 * This requires that microstep control pins be connected in addition to STEP,DIR
 *
 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

#define STEP 8
#define DIR 9
// #define ENABLE 13 // optional (just delete ENABLE from everywhere if not used)

/*
 * Choose one of the sections below that match your board
 */

// #include "DRV8834.h"
// #define M0 10
// #define M1 11
// DRV8834 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, M0, M1);

// #include "A4988.h"
// #define MS1 10
// #define MS2 11
// #define MS3 12
// A4988 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, MS1, MS2, MS3);

#include "DRV8825.h"
#define MODE0 5
#define MODE1 6
#define MODE2 7
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, MODE0, MODE1, MODE2);

// #include "DRV8880.h"
// #define M0 10
// #define M1 11
// #define TRQ0 6
// #define TRQ1 7
// DRV8880 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, M0, M1, TRQ0, TRQ1);

// #include "BasicStepperDriver.h" // generic
// BasicStepperDriver stepper(DIR, STEP);

#define RIGHT 2
#define LEFT 3

int r = 0;
int l = 0;
int microstep = 32;
int stp = 1;

void setup() {
    /*
     * Set target motor RPM.
     */
    stepper.begin(RPM);
    //stepper.enable();

    pinMode(RIGHT, INPUT);
    pinMode(LEFT, INPUT);

    Serial.begin(9600);
    
    // set current level (for DRV8880 only). 
    // Valid percent values are 25, 50, 75 or 100.
    // stepper.setCurrent(100);
}

void loop() {
    //delay(1000);

    r = digitalRead(RIGHT);
    l = digitalRead(LEFT);

    Serial.print(l);
    Serial.print(r);
    Serial.println();

    /*
     * Moving motor in full step mode is simple:
     */
    //stepper.setMicrostep(1);  // Set microstep mode to 1:1

    // One complete revolution is 360°
    //stepper.rotate(360);     // forward revolution
    //stepper.rotate(-360);    // reverse revolution

    // One complete revolution is also MOTOR_STEPS steps in full step mode
    //stepper.move(MOTOR_STEPS);    // forward revolution
    //stepper.move(-MOTOR_STEPS);   // reverse revolution

    /*
     * Microstepping mode: 1, 2, 4, 8, 16 or 32 (where supported by driver)
     * Mode 1 is full speed.
     * Mode 32 is 32 microsteps per step.
     * The motor should rotate just as fast (at the set RPM),
     * but movement precision is increased, which may become visually apparent at lower RPMs.
     */
    stepper.setMicrostep(microstep);   // Set microstep mode to 1:8

    // In 1:8 microstepping mode, one revolution takes 8 times as many microsteps
    if(r != l){
        if(r == HIGH){
            stepper.move(microstep * stp);    // forward revolution
        }
        else{
            stepper.move(-microstep * stp);   // reverse revolution
        }
    }
    
    
    
    // One complete revolution is still 360° regardless of microstepping mode
    // rotate() is easier to use than move() when no need to land on precise microstep position
    //stepper.rotate(360);
    //stepper.rotate(-360);

    delay(5);
}
