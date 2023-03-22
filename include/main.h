/**
 * @file main.h
 * @author Artur Mukhamedyarov (pozitp.code@gmail.com)
 * @brief Main module for main code
 * License: GNU General Public License v3.0 (see LICENSE) 
 * @version 0.1
 * @date 2023-03-21
 * 
 * @copyright pozitp (c) 2023
 * 
 */

#pragma once
#include <Arduino.h>
#include <NewPing.h>
#include <HTInfraredSeeker.h>
#include <MPU9250.h>
#include "logging.h"
#include "motor.h"

//* Set there length of field
#define FIELD_LENGTH 241

// Declare GY-91 sensor
MPU9250 mpu;

// Declare US sensors
NewPing sonar[4] = {
  NewPing(30, 30, 270),
  NewPing(28, 28, 270), 
  NewPing(39, 39, 270),
  NewPing(29, 29, 270)
};

// Declare necessary variables
uint16_t tmr;
double X, Y, ANG;
float COMP, COMP_CURR, OFFSET;
InfraredResult InfraredBall;
uint8_t L_US, R_US, B_US, F_US;
bool BUTTON_STATE = false;

/**
 * @brief Trigger updating values from US sensors, update coordinates and angle to goalpost, update IR sensor
 * 
 */
void trigger_infoupdate() {
  uint16_t ms = millis() & 0xFFFF;
  if (ms - tmr > 50) {
    tmr = ms;
    L_US = sonar[0].ping_cm() + 9.5;
    R_US = sonar[1].ping_cm() + 9.5;
    B_US = sonar[2].ping_cm() + 9.5;
    F_US = sonar[3].ping_cm();
    X = (L_US - R_US) / 2;
    Y = FIELD_LENGTH - B_US;
    ANG = degrees(atan(X / Y));
    InfraredBall = InfraredSeeker::ReadAC();
  } 
}

/**
 * @brief Trigger updating values from compass
 * 
 */
void trigger_compass() {
  mpu.update();
  COMP = degrees(atan2(mpu.getMagY(), mpu.getMagX()));
}

/**
 * @brief Function to determine the position to the goalpost and drive to it
 * 
 */
void find_position() {

  // Run forward if robot in center
  if (ANG == 0) {
    while (!(F_US <= 13)) {
      trigger_infoupdate();
      trigger_compass();
      if (F_US <= 13) break;
      movement(0, 180, COMP - OFFSET);
    }
  }

  if(ANG != 0) {
    trigger_compass();

    // Center view of robot forward to wall
    while (!((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1))) {
      trigger_compass();
      if ((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1)) break;
      movement_rotate_on_spot(COMP - OFFSET);
    }
    movement_stop();

    trigger_infoupdate();
    trigger_compass();

    // Rotate to angle that center robot to goalpost
    while (!(((-ANG) - 1) <= (COMP - OFFSET)) && ((COMP - OFFSET) <= ((-ANG) + 1))) {
      trigger_compass();
      if ((((-ANG) - 1) <= (COMP - OFFSET)) && ((COMP - OFFSET) <= ((-ANG) + 1))) break;
      movement_rotation(-ANG, 45);
    }
    movement_stop();

    trigger_compass();
    COMP_CURR = COMP;
    
    // Go to goalpost before we reach it
    while (!(F_US <= 13)) {
      trigger_infoupdate();
      trigger_compass();
      if (F_US <= 13) break;
      movement(0, 180, COMP - COMP_CURR);
    }
    movement_stop();

    // Center view of robot forward to wall
    while (!((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1))) {
      trigger_compass();
      movement_rotate_on_spot(COMP - OFFSET);
    } 
    movement_stop();

    while (!((-2 <= X) && (X <= 2))) {
      trigger_infoupdate();
      trigger_compass();

      if (X > 0) {
        if ((-2 <= X) && (X <= 2)) break;
        movement(90, 180, COMP - OFFSET);
      } 
      if (X < 0) {
        if ((-2 <= X) && (X <= 2)) break;
        movement(270, 180, COMP - OFFSET);
      }
    } 
    movement_stop();

    // Run back to starting point
    while (B_US > 40)
    {
      trigger_infoupdate();
      trigger_compass();
      movement(180, 180, COMP - OFFSET);
    }

    
    //! Temprorary method: stop robot
    movement_stop();
/*     for(;;) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    } */
    
      // Center view of robot forward to wall
    while (!((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1))) {
      trigger_compass();
      movement_rotate_on_spot(COMP - OFFSET);
    }
    movement_stop();
  }
}