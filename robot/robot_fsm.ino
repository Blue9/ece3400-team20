/**
 * ECE 3400 Team 20. Main maze navigation code.
 * September 8, 2018.
 *
 * Copyright Team 20 2018.
 */
#include <Servo.h>

#define RIGHT_COLOR_PIN A1
#define LEFT_COLOR_PIN A2
#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 11
#define SENSOR_THRESHOLD 750  // Determined experimentally.

#define LEFT_VELOCITY_MIN 0
#define LEFT_VELOCITY_MAX 180
#define RIGHT_VELOCITY_MIN 180
#define RIGHT_VELOCITY_MAX 0
#define MAX_SPEED 0.1

int (*states[])(void) = {
    move_forward,
    adjust_left,
    adjust_right,
    turn,
    forward_until_past_intersection,
    start_turn,
    wait_until_turn_end,  // This trailing comma is here for the auto-formatter.
};

void setup() {
  // TODO
}
void loop() {
  // TODO
}

int move_forward() {
  set_left(1);
  set_right(1);
  return 0;
}

int adjust_left() {
  set_left(0);
  return 0;
}

int adjust_right() {
  set_right(0);
  return 0;
}

int turn() {
  // TODO
  return 0;
}

int forward_until_past_intersection() {
  // TODO
  return 0;
}

int start_turn() {
  adjust_left(0);
  return 0;
}

int wait_until_turn_end() {
  // TODO
  return 0;
}