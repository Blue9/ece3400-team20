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
  pinMode(FRONT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  pinMode(FRONT_DISTANCE_PIN, INPUT);
  pinMode(RIGHT_DISTANCE_PIN, INPUT);
  Serial.begin(9600);
  // Start servos for a short period of time.
  next_state = WAIT_FOR_TONE;
  set_left(0);
  set_right(0);
}

void loop() {
  next_state = handle_next_state();
  Serial.println(analogRead(A4));
  digitalWrite(RIGHT_LED_PIN, right_wall() * HIGH);
  digitalWrite(FRONT_LED_PIN, front_wall() * HIGH);
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
