/**
 * @file HTInfraredSeeker.h
 * @author Artur Mukhamedyarov (pozitp.code@gmail.com)
 * @brief A library for the HiTechnic IRSeeker V2 infrared sensor. Originally created by Birk Blechschmidt in August 1, 2013 and released into the public domain.
 * This version license: GNU General Public License v3.0 (see LICENSE) 
 * Original library released into the public domain
 * @version 1.1
 * @date 2023-03-21
 * 
 * Special thanks for Birk Blechschmidt for sharing findings with FOSS community!
 * 
 * @copyright pozitp (c) 2023
 * 
 */

#ifndef _HTINFRAREDSEEKER_H_
#define _HTINFRAREDSEEKER_H_

#include <Arduino.h>

// Structure of returning values
struct InfraredResult {
	byte Direction;
	byte Strength;
};

class InfraredSeeker {
public:
	/**
	 * @brief Initialize IRSeeker on default I2C address
	 * 
	 */
	static void Initialize();
	/**
	 * @brief Test of connection 
	 * 
	 * @return boolean If connected and runs correctly return true
	 */
	static boolean Test();
	/**
	 * @brief Function, reads AC raw values
	 * 
	 * @param buffer Provide pointer to bufffer, result will be there
	 */
	static void ReadACRaw(byte* buffer);
	/**
	 * @brief Function, reads DC raw values
	 * 
	 * @param buffer Provide pointer to bufffer, result will be there
	 */
	static void ReadDCRaw(byte* buffer);
	/**
	 * @brief Read modulated (PWM) IR signal
	 * 
	 * @return InfraredResult Returns values to InfraedResult struct
	 */
	static InfraredResult ReadAC();
	/**
	 * @brief Read unmodulated IR signal
	 * 
	 * @return InfraredResult Returns values to InfraedResult struct
	 */
	static InfraredResult ReadDC();
	/**
	 * @brief Formats traditional angle values to degree
	 * 
	 * @param Direction Provide value of direction from sensor
	 * @return int Returns degrees
	 */
	static int DirectionAngle(byte Direction);
private:
	/**
	 * @brief Private function, formats raw values to readable format
	 * 
	 * @param buffer Provide pointer to bufffer, result will be there
	 * @return InfraredResult Write to InfraedResult
	 */
	static InfraredResult PopulateValues(byte* buffer);
	/**
	 * @brief Private function to read I2C
	 * 
	 * @param OffsetAddress Set address to request from
	 * @param buffer Provide pointer to bufffer, result will be there
	 */
	static void ReadValues(byte OffsetAddress, byte* buffer);
	/**
	 * @brief Defining constant that contains I2C adress of IRSeeker
	 * 
	 */
	static const int Address = 0x10 / 2; // Divide by two as 8bit-I2C address is provided
};

#endif // _HTINFRAREDSEEKER_H_