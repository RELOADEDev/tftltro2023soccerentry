/**
 * @file PID.h
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

#ifndef _PID_H_
#define _PID_H_

#include <Arduino.h>

class PID_lib {
public:
	/**
	 * @brief PID calculations function
	 * 
	 * @param speeds Provide speed for motors
	 * @param setpoint Set default (0)
	 * @param input Provide values from compass, modified or not
	 * @return int Return motor compensation for self-corretion
	 */
	static int PID(int16_t speeds[4], int setpoint, int input);
};

#endif // _PID_H_