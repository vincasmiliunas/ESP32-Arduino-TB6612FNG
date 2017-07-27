#include <TB6612FNG.h>

// 27 - Standby pin
// 14 - AIN1 pin
// 12 - AIN2 pin
// 13 - PWMA pin
// 26 - BIN1 pin
// 25 - BIN2 pin
// 33 - PWMB pin
// These pins are in a straight line on the left side of WEMOS LOLIN32 board (skip 34 and 35 as they are input-only).
// To reverse forward motor direction, switch the AIN1 and AIN2 or BIN1 and BIN2 pin numbers.
Tb6612fng motors(27, 14, 12, 13, 26, 25, 33);
// Tb6612fng motors(27, 12, 14, 13, 25, 26, 33); // Reversed forward motor direction.

void setup() {
  Serial.begin(115200);
  motors.begin();
}

void loop() {
  Serial.printf("Loop: %d\n", millis());
  // 500ms forwards
  motors.drive(0.5, 500);
  // 500ms backwards
  motors.drive(-0.5, 500);
  // 500ms left
  motors.drive(-1.0, 1.0, 500);
  // 500ms right
  motors.drive(1.0, -1.0, 500);
  delay(1000);
}
