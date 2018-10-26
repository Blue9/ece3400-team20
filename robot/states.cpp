#include <USBAPI.h>

#include "array_update.h"
#include "constants.h"
#include "radio.h"
#include "robot_state.h"
#include "states.h"
#include "turn_states.h"
#include "util.h"

function_t states[] = {
  wait_for_tone,
  move_forward,
  adjust_left,
  adjust_right,
  forward_until_past_intersection,
  handle_intersection,
  early_turn,
  mid_turn,
  late_turn,
  start_180,
  mid_180,
  late_180
};

int next_state;
int turn_direction = 1; // 0 == left, 1 == right
function_t on_white[] = {left_on_white, right_on_white};
function_t on_black[] = {left_on_black, right_on_black};

int handle_next_state() {
  return states[next_state]();  // Returns a function.
}

int wait_for_tone() {
  Serial.println("wait for tone");
  if (!audioFFT() && !override_pressed()) return WAIT_FOR_TONE;
  return MOVE_FORWARD;
}

int move_forward() {
  Serial.println("move forward");
  set_left(1);
  set_right(1);
  if (only_left_on_white()) return ADJUST_LEFT;
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (both_on_white()) {
    // At intersection, update current position data
    return FORWARD_UNTIL_PAST_INTERSECTION;
  }
  return MOVE_FORWARD;
}

int adjust_left() {
  Serial.println("adjust left");
  set_left(0.5);
  set_right(1);
  if (only_left_on_white()) return ADJUST_LEFT;
  if (neither_on_white()) return MOVE_FORWARD;
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  return ADJUST_LEFT;
}

int adjust_right() {
  Serial.println("adjust right");
  set_right(0.5);
  set_left(1);
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (neither_on_white()) return MOVE_FORWARD;
  if (only_left_on_white()) return ADJUST_LEFT;
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  return ADJUST_RIGHT;
}

int forward_until_past_intersection() { // must turn after this state
  Serial.print("forward_until_past_intersection ");
  set_left(1);
  set_right(1);
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  return HANDLE_INTERSECTION;
}

int handle_intersection() {
  Serial.println("handle_intersection");
  set_left(0);
  set_right(0);
  update_walls();
  update_robot_position();
  transmit_msg();
  array_update(x_robot, y_robot, west, north, east, south, 0, 0);
  if (!right_wall()) turn_direction = 1;
  else if (front_wall() && !left_wall()) turn_direction = 0;
  if (right_wall() && !front_wall()) return MOVE_FORWARD;
  else if (right_wall() && left_wall() && front_wall()) return START_180;
  return EARLY_TURN;
}
