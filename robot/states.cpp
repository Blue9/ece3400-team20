#include "states.h"
#include "constants.h"
#include "util.h"
#include <USBAPI.h>

int (*next_state)(void);

int (*get_state())(void) {
  return next_state;  // Returns a function.
}

int (*move_forward())(void) {
  Serial.println("move forward");
  set_left(1);
  set_right(1);
  if (only_left_on_white()) return adjust_left;
  if (only_right_on_white()) return adjust_right;
  if (both_on_white()) return forward_until_past_intersection;
  return move_forward;
}

int (*adjust_left())(void) {
  Serial.println("adjust left");
  set_left(0);
  if (only_left_on_white()) return adjust_left;
  if (neither_on_white()) return move_forward;
  if (only_right_on_white()) return adjust_right;
  if (both_on_white()) return forward_until_past_intersection;
  return adjust_left;
}

int (*adjust_right())(void) {
  Serial.println("adjust right");
  set_right(0);
  if (only_right_on_white()) return adjust_right;
  if (neither_on_white()) return adjust_left;
  if (only_left_on_white()) return move_forward;
  if (both_on_white()) return forward_until_past_intersection;
  return adjust_right;
}

int (*forward_until_past_intersection())(void) {
  Serial.println("forward_until_past_intersection");
  set_left(1);
  set_right(1);
  if (both_on_white()) return forward_until_past_intersection;
  if (!left_on_white()) return start_turn;
  return forward_until_past_intersection;
}

int (*start_turn())(void) {
  Serial.println("start turn");
  set_left(0);
  set_right(1);
  if (!left_on_white()) return start_turn;
  if (left_on_white()) return wait_until_turn_end;
  return start_turn;
}

int (*wait_until_turn_end())(void) {
  Serial.println("wait_until_turn_end");
  set_left(0);
  set_right(1);
  if (left_on_white()) return wait_until_turn_end;
  if (!left_on_white()) return move_forward;
  return wait_until_turn_end;
}

int (*check_for_walls())(void) {
  Serial.println("check_for_walls");
  if (both_wall()) {
    front_led_on();
    right_led_on();
    return start_turn; // Need to turn left & go forward
  }
  if (right_wall()) {
    right_led_on();
    return move_forward;
  }
  if (front_wall()) {
    front_led_on();
    return start_turn; // Need to turn right & go forward
  }
  if (neither_wall()) {
    front_led_off();
    right_led_off();
    return start_turn; // Need to turn right & go forward
  }
}
