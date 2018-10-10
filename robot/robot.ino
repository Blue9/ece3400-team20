/**
 * ECE 3400 Team 20. Main maze navigation code.
 * September 8, 2018.
 *
 * Copyright Team 20 2018.
 */
#include "constants.h"
#include "servos.h"
#include "states.h"
#include "util.h"

Servo right_servo;
Servo left_servo;

void setup() {
  right_servo.attach(RIGHT_SERVO_PIN);
  left_servo.attach(LEFT_SERVO_PIN);
  pinMode(RIGHT_COLOR_PIN, INPUT);
  pinMode(LEFT_COLOR_PIN, INPUT);
  Serial.begin(9600);
  // Start servos for a short period of time.
  Serial.println("1");
  next_state = MOVE_FORWARD;
  Serial.println("2");
  delay(500);
}

void loop() {
  next_state = handle_next_state();
  Serial.println("3");
  Serial.print(left_servo.read());
  Serial.print(" ");
  Serial.println(right_servo.read());
  delay(10);
}
