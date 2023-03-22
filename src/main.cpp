/**
 * @file logging.h
 * @author Artur Mukhamedyarov (pozitp.code@gmail.com)
 * @brief Main code
 * License: GNU General Public License v3.0 (see LICENSE) 
 * @version 0.1
 * @date 2023-03-21
 * 
 * @copyright pozitp (c) 2023
 * 
 */

#include "main.h"

int main() {
  init();

  // Initial functions
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  mpu.setup(0x68);
  // Calibrate GY-91
  mpu.verbose(true);
  mpu.calibrateMag();
  for (int i = 0; i < 10; i++) {
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  Serial.println("Done.");
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);
  mpu.calibrateAccelGyro();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  mpu.verbose(false);
  InfraredSeeker::Initialize();
  // Set angle to enemy's goalpost
  uint32_t now = millis();
  while (millis() - now < 500) {
    trigger_infoupdate();
    trigger_compass();
    OFFSET = COMP;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

  // Main loop
  for (;;) {
    trigger_infoupdate();

    while (InfraredBall.Direction == 0) {
        trigger_infoupdate();
        while (!((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1))) {
          trigger_compass();
          if ((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1)) break;
          movement_rotate_on_spot(COMP - OFFSET);
        }
        movement_stop();
    }

    while (InfraredBall.Direction != 0) {
      while (InfraredBall.Direction != 5) {
        trigger_infoupdate();
        if (InfraredBall.Direction > 5) {
          trigger_infoupdate();
          movement_rotation(1, 30);
          if (digitalRead(26) == LOW) {
            movement_stop();
            find_position();
            break;
          }
        }
        if (InfraredBall.Direction < 5) {
          trigger_infoupdate();
          movement_rotation(-1, 30);
          if (digitalRead(26) == LOW) {
            movement_stop();
            find_position();
            break;
          }
        }
      }

      while (InfraredBall.Direction == 5) {
        trigger_infoupdate();
        trigger_compass();
        // TODO: Protection of scoring an own goal
        // if (InfraredBall.Strength >= 130 && (-35 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= -15)) {
        //   if ((-1 <= (COMP - OFFSET)) && ((COMP - OFFSET) <= 1)) break;
        //   motorFL.run(180);
        //   motorFR.run(-180);
        //   motorRR.run(180);
        //   motorRL.run(180);
        // }
        movement_straight(60);
        if (digitalRead(26) == LOW) {
          movement_stop();
          find_position();
          break;
        }
      }
    }
  }
} 