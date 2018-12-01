#include "util.h"
#include "constants.h"
#include "states.h"
#include "robot_state.h"
#include "turn_states.h"
#include <USBAPI.h>

int early_turn() {
  // Wait until the corresponding line sensor is off white.
  Serial.println("early_turn");
  update_direction(turn_direction);
  set_left(turn_direction);
  set_right(1 - turn_direction);
  if (on_white[turn_direction]()) return EARLY_TURN;
  if (on_black[turn_direction]()) return MID_TURN;
}

int mid_turn() {
  // Wait until the corresponding line sensor is back on white.
  Serial.println("mid_turn");
  set_left(turn_direction);
  set_right(1 - turn_direction);
  if (on_black[turn_direction]()) return MID_TURN;
  if (on_white[turn_direction]()) return LATE_TURN;
}

int late_turn() {
  // Wait until the line sensor is back off white.
//  Serial.println("late_turn");
  set_left(turn_direction);
  set_right(1 - turn_direction);
  if (on_white[turn_direction]()) return LATE_TURN;
  if (on_black[turn_direction]()) return MOVE_FORWARD;
}

int start_180() {
//  Serial.println("start_180");
  current_direction = (2 - current_direction);
  set_left(-1);
  set_right(1);
  if (left_on_white()) return START_180;
  if (left_on_black()) return MID_180;
}

int mid_180() {
//  Serial.println("mid_180");
  set_left(-1);
  set_right(1);
  if (left_on_black()) return MID_180;
  if (left_on_white()) {/*Serial.println(analogRead(LEFT_COLOR_PIN));*/ return LATE_180;};
}

int late_180() {
//  Serial.println("late_180");
  set_left(-1);
  set_right(1);
  while (left_on_white()) /*Serial.println(analogRead(LEFT_COLOR_PIN))*/;
  while (left_on_black());
  while (left_on_white());
  return MOVE_FORWARD;
}
