/**
 * ECE 3400 Team 20. Main maze navigation code.
 * September 8, 2018.
 *
 * Copyright Team 20 2018.
 */
#include "constants.h"
#include "servos.h"
#include "states.h"
#include "robot_state.h"
#include "radio.h"
#include "util.h"

Servo right_servo;
Servo left_servo;

void setup() {
  right_servo.attach(RIGHT_SERVO_PIN);
  left_servo.attach(LEFT_SERVO_PIN);
  pinMode(RIGHT_COLOR_PIN, INPUT);
  pinMode(LEFT_COLOR_PIN, INPUT);
  pinMode(FRONT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);

  pinMode(WALL_MUX_PIN_OUT1, OUTPUT);
  pinMode(WALL_MUX_PIN_OUT2, OUTPUT);

  pinMode(OVERRIDE_SWITCH_PIN, INPUT);
  Serial.begin(9600);
  radio_setup();
  // Start servos for a short period of time.
  next_state = WAIT_FOR_TONE;
  set_left(0);
  set_right(0);
}

void loop() {
  // while (1) {
//     Serial.print(analogRead(LEFT_COLOR_PIN));
//     Serial.print(" ");
//     Serial.println(analogRead(RIGHT_COLOR_PIN));
  // }
  next_state = handle_next_state();
  digitalWrite(RIGHT_LED_PIN, right_wall() * HIGH);
  digitalWrite(FRONT_LED_PIN, front_wall() * HIGH);
  digitalWrite(LEFT_LED_PIN, left_wall() * HIGH);
//  Serial.print(x_robot);
//  Serial.print(" ");
//  Serial.print(y_robot);
//  Serial.println();
  delay(1);
//  while(1) { // reduces jitter
//    right_servo.write(90);
//  left_servo.write(90);
//      bool audio    = audioFFT();
//      bool optical  = opticalFFT();
//      Serial.print("Optical: ");
//      Serial.print(optical);
//      Serial.print("\tAudio: ");
//      Serial.println(audio);
//  }
}
