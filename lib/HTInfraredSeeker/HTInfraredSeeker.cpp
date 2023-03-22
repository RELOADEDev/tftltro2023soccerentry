/**
 * @file HTInfraredSeeker.cpp
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

#include <Arduino.h>
#include <Wire.h>
#include "HTInfraredSeeker.h"

/**
 * @brief Initialize IRSeeker on default I2C address
 * 
 */
void InfraredSeeker::Initialize()
{
	Wire.begin();
	Wire.beginTransmission(InfraredSeeker::Address);
	Wire.write(0x00);
	Wire.endTransmission();
	while(Wire.available() > 0)
	Wire.read();
}

/**
 * @brief Test of connection 
 * 
 * @return boolean If connected and runs correctly return true
 */
boolean InfraredSeeker::Test() {
	Wire.beginTransmission(InfraredSeeker::Address);
	Wire.write(0x08);
	Wire.endTransmission();
	Wire.requestFrom(InfraredSeeker::Address, 16);
	char Manufacturer_Model[16];
	while(Wire.available() < 16);
	for(byte i=0; i < 16; i++) {
		Manufacturer_Model[i] = Wire.read();
		Serial.write(Manufacturer_Model[i]);
	}
	while(Wire.available() > 0)
	Wire.read();
	return strncmp(Manufacturer_Model, "HiTechncNewIRDir", 16) == 0;
}

/**
 * @brief Private function to read I2C
 * 
 * @param OffsetAddress Set address to request from
 * @param buffer Provide pointer to bufffer, result will be there
 */
void InfraredSeeker::ReadValues(byte OffsetAddress, byte* buffer) {
	Wire.beginTransmission(InfraredSeeker::Address);
	Wire.write(OffsetAddress);
	Wire.endTransmission();
	Wire.requestFrom(InfraredSeeker::Address, 6);
	while(Wire.available() < 6);
	for(byte i = 0; i < 6; i++) {
		buffer[i] = Wire.read();
	}
	while(Wire.available() > 0)
	Wire.read();
}

/**
 * @brief Function, reads AC raw values
 * 
 * @param buffer Provide pointer to bufffer, result will be there
 */
void InfraredSeeker::ReadACRaw(byte* buffer) {
	ReadValues(0x49, buffer);
}

/**
 * @brief Function, reads DC raw values
 * 
 * @param buffer Provide pointer to bufffer, result will be there
 */
void InfraredSeeker::ReadDCRaw(byte* buffer) {
	ReadValues(0x42, buffer);
}

/**
 * @brief Private function, formats raw values to readable format
 * 
 * @param buffer Provide pointer to bufffer, result will be there
 * @return InfraredResult Write to InfraedResult
 */
InfraredResult InfraredSeeker::PopulateValues(byte* buffer) {
	InfraredResult Data;
	Data.Direction = buffer[0];
	if(buffer[0] != 0) {
		if(buffer[0] % 2 == 0) {
			Data.Strength = (buffer[buffer[0] / 2] + buffer[buffer[0] / 2 + 1]) / 2;
		} else {
			Data.Strength = buffer[buffer[0] / 2 + 1];
		}
	} else {
		Data.Strength = 0;
	}
	return Data;
}

/**
 * @brief Read modulated (PWM) IR signal
 * 
 * @return InfraredResult Returns values to InfraedResult struct
 */
InfraredResult InfraredSeeker::ReadAC() {
	byte buffer[6];
	ReadACRaw(buffer);
	return PopulateValues(buffer);
}

/**
 * @brief Read unmodulated IR signal
 * 
 * @return InfraredResult Returns values to InfraedResult struct
 */
InfraredResult InfraredSeeker::ReadDC() {
	byte buffer[6];
	ReadDCRaw(buffer);
	return PopulateValues(buffer);
}

/**
 * @brief Formats traditional angle values to degree
 * 
 * @param Direction Provide value of direction from sensor
 * @return int Returns degrees
 */
int InfraredSeeker::DirectionAngle(byte Direction) {
	return Direction * 30 - 150;
}