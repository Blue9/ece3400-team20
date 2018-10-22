#include "states.h"
#include "robot_state.h"
#include "radio.h"
#include "constants.h"
#include "util.h"
#include <USBAPI.h>

function_t states[] = {
  wait_for_tone,
  move_forward,
  adjust_left,
  adjust_right,
  forward_until_past_intersection,
  handle_intersection,
  start_turn,
  wait_until_turn_end,
  undo_turn
};

int next_state;
int turn_direction = 1; // 0 == left, 1 == right
function_t turn_status[] = {left_on_white, right_on_white};


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
  Serial.println(turn_direction);
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
  if (!right_wall()) turn_direction = 1;
  else if (front_wall() && !left_wall()) turn_direction = 0;

  transmit_msg();
  if (right_wall() && !front_wall()) return MOVE_FORWARD;
  else if (right_wall() && left_wall() && front_wall()) return UNDO_TURN;
  update_direction(turn_direction);
  return START_TURN;
}

int start_turn() {
  Serial.println("start_turn");
  set_left(turn_direction);
  set_right(1 - turn_direction);
  delay(100);
  if (!turn_status[turn_direction]()) return START_TURN;
  if (turn_status[turn_direction]()) return WAIT_UNTIL_TURN_END;
  return START_TURN;
}

int wait_until_turn_end() {
  Serial.println("wait_until_turn_end");
  set_left(turn_direction);
  set_right(1 - turn_direction);
  if (turn_status[turn_direction]()) return WAIT_UNTIL_TURN_END;
  if (!turn_status[turn_direction]()) return MOVE_FORWARD;
  return WAIT_UNTIL_TURN_END;
}

int undo_turn() {
  set_left(-1);
  set_right(1);
  delay(750);
  while (!left_on_white());
  while (left_on_white());
  reverse_direction();
  return MOVE_FORWARD;
}
