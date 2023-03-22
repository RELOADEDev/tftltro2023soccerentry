/**
 * @file PID.cpp
 * @author Artur Mukhamedyarov (pozitp.code@gmail.com)
 * @brief Personal library for PID control of motors. 
 * This version license: GNU General Public License v3.0 (see LICENSE)
 * Original library released into the public domain
 * 
 * This is a fork of XLCTeam PID control, so there is special thanks them for sharing findings with FOSS community!
 * 
 * @version 1.1
 * @date 2023-03-21
 * 
 * @copyright pozitp (c) 2023
 * 
 */

#include <Arduino.h>
#include "PID.h"

int PID_P = 0;
long PID_I = 0;
int PID_D = 0;
//////////////////////////////////////////////////////
#define P 3
#define I 0.05
#define D 10
//////////////////////////////////////////////////////
#define max_I 100*100
#define min_I -100*100
//////////////////////////////////////////////////////
int out;
int error;

int last_input = 0;
long last_time = 0;
int last_error = 0;
long now = 0;
int errors[3] = { 0 , 0, 0};

/**
 * @brief PID calculations function
 * 
 * @param speeds Provide speed for motors
 * @param setpoint Set default (0)
 * @param input Provide values from compass, modified or not
 * @return int Return motor compensation for self-corretion
 */
int PID_lib::PID(int16_t speeds[4], int setpoint, int input) {

  int16_t maximal_value = -450;
  int16_t minimal_value = 450;
  for (int16_t i = 0; i < 4; i++) {
    if (speeds[i] < minimal_value) {
      minimal_value = speeds[i];
    }
    if (maximal_value < speeds[i])
      maximal_value = speeds[i];
  }
  maximal_value = 93 - maximal_value;
  minimal_value = -93 - minimal_value;

  error = setpoint - input;

  errors[2] =   errors[1];
  errors[1] =   errors[0];
  errors[0] =   error;

  now = millis();
  if (error != 0) {
    PID_P = error * P;

    PID_I += ((now - last_time) * error ) * I ;
    if (PID_I < min_I) {
      PID_I = min_I;
    } else if (PID_I > max_I) {
      PID_I = max_I;
    }

    PID_D = (errors[1] - error) * D; //TODO timer D move

    out = PID_P + (PID_I / 100) - PID_D;
    if (out > maximal_value) {
      out = maximal_value;
    } else if (out < minimal_value) {
      out = minimal_value;
    }

  } else {
    PID_I = 0;
  }
  last_time = now;
  last_error = error;
  last_input = input;

  return out;
}