/**
 * @file logging.h
 * @author Artur Mukhamedyarov (pozitp.code@gmail.com)
 * @brief Module for draw logs on graph (Teleplot)
 * License: GNU General Public License v3.0 (see LICENSE) 
 * @version 0.1
 * @date 2023-03-21
 * 
 * @copyright pozitp (c) 2023
 * 
 */

#pragma once
#include <Arduino.h>

/**
 * @brief Print formatted value to show on graph (use Teleplot)
 * 
 * @param var Name of parameter that will be showed on graph
 * @param val Value of specified parameter (double, int, byte (uint8_t))
 */
void logger_graph(String var, double val) {
  Serial.print(">" + var + ":");
  Serial.println(val);
}

/**
 * @brief Print formatted value to show on graph (use Teleplot)
 * 
 * @param var Name of parameter that will be showed on graph
 * @param val Value of specified parameter (double, int, byte (uint8_t))
 */
void logger_graph(String var, int val) {
  Serial.print(">" + var + ":");
  Serial.println(val);
}

/**
 * @brief Print formatted value to show on graph (use Teleplot)
 * 
 * @param var Name of parameter that will be showed on graph
 * @param val Value of specified parameter (double, int, byte (uint8_t))
 */
void logger_graph(String var, uint8_t val) {
  Serial.print(">" + var + ":");
  Serial.println(val);
}
