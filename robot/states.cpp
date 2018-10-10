#include "states.h"
#include "constants.h"
#include "util.h"
#include <USBAPI.h>

state_function_t states[] = {
  move_forward,
  adjust_left,
  adjust_right,
  forward_until_past_intersection,
  start_turn,
  wait_until_turn_end
};

int next_state;

int handle_next_state() {
  return states[next_state]();  // Returns a function.
}

int move_forward() {
  Serial.println("move forward");
  set_left(1);
  set_right(1);
  if (only_left_on_white()) return ADJUST_LEFT;
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  return MOVE_FORWARD;
}

int adjust_left() {
  Serial.println("adjust left");
  set_left(0);
  if (only_left_on_white()) return ADJUST_LEFT;
  if (neither_on_white()) return MOVE_FORWARD;
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  return ADJUST_LEFT;
}

int adjust_right() {
  Serial.println("adjust right");
  set_right(0);
  if (only_right_on_white()) return ADJUST_RIGHT;
  if (neither_on_white()) return ADJUST_LEFT;
  if (only_left_on_white()) return MOVE_FORWARD;
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  return ADJUST_RIGHT;
}

int forward_until_past_intersection() {
  Serial.println("forward_until_past_intersection");
  set_left(1);
  set_right(1);
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION;
  if (!left_on_white()) return START_TURN;
  return FORWARD_UNTIL_PAST_INTERSECTION;
}

int start_turn() {
  Serial.println("start turn");
  set_left(0);
  set_right(1);
  if (!left_on_white()) return START_TURN;
  if (left_on_white()) return WAIT_UNTIL_TURN_END;
  return START_TURN;
}

int wait_until_turn_end() {
  Serial.println("wait_until_turn_end");
  set_left(0);
  set_right(1);
  if (left_on_white()) return WAIT_UNTIL_TURN_END;
  if (!left_on_white()) return MOVE_FORWARD;
  return WAIT_UNTIL_TURN_END;
}
