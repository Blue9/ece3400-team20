#include "states.h"
#include "constants.h"
#include "util.h"
#include <USBAPI.h>

function_t states[] = {
  move_forward,
  adjust_left,
  adjust_right,
  forward_until_past_intersection,
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

int move_forward() {
  Serial.println("move forward");
  set_left(1);
  set_right(1);
  if (only_left_on_white()) return ADJUST_LEFT;
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (both_on_white() && (front_wall() || !right_wall() || opticalFFT())) {
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
  if (both_on_white() && (front_wall() || !right_wall() || opticalFFT())) return FORWARD_UNTIL_PAST_INTERSECTION;
  return ADJUST_LEFT;
}

int adjust_right() {
  Serial.println("adjust right");
  set_right(0.5);
  set_left(1);
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (neither_on_white()) return MOVE_FORWARD;
  if (only_left_on_white()) return ADJUST_LEFT;
  if (both_on_white() && (front_wall() || !right_wall() || opticalFFT())) return FORWARD_UNTIL_PAST_INTERSECTION;
  return ADJUST_RIGHT;
}

int forward_until_past_intersection() { // must turn after this state
  Serial.println("forward_until_past_intersection");
  set_left(1);
  set_right(1);
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  if (!right_wall()) turn_direction = 1;
  else turn_direction = 0;
  if (!turn_status[turn_direction]()) {
    set_left(0);
    set_right(0);
    delay(10);
    return START_TURN;
  }
  return FORWARD_UNTIL_PAST_INTERSECTION;
}

int start_turn() {
  Serial.println("start turn");
  set_left(turn_direction);
  set_right(1 - turn_direction);
  if (!turn_status[turn_direction]()) return START_TURN;
  if (turn_status[turn_direction]()) {
    // Before completing turn, check for a dead end.
    Serial.print("fonrt walll???????????  ");
    Serial.println(front_wall());
    if (front_wall() && turn_direction == 0) return UNDO_TURN;
    else return WAIT_UNTIL_TURN_END;
  }
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
  // We were turning left, but need to reverse that.
  byte del = 50;
  Serial.println("undo turn");
  set_left(0);
  set_right(-1);
  while (!right_on_white());
  delay(del);
  while (right_on_white());
  set_left(1);
  set_right(-1);
  delay(del);
  while (!right_on_white());
  delay(del);
  while (right_on_white());
  delay(del);
  while (!right_on_white());
  delay(del);
  while (right_on_white());
  return MOVE_FORWARD;
}
