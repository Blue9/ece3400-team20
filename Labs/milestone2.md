# Milestone 2

## Maze exploration using Right Hand Rule
 
To add maze exploration functionality using the right hand rule, we incorporated the following logic:
```
  if no_right_wall:
      turn_right()
  else if no_front_wall: // right wall is detected but no front wall is detected
      go_straight()
  else:
      turn_left() // right wall and front wall is detected 
```
We try to turn right at every intersection, and if the right side is blocked, we try moving forward. We added two short distance sensors for wall detection on the right and front of the robot in addition to the IR sensor explored in Lab 2. We also added two indicator LEDs to show whether the robot detects a front or right wall. Front and right wall detection uses an experimentally determined threshold value from the distance sensors. We found 100 to be a good threshold for the left wall sensor and 200 to be a good threshold for the right sensor. In case the robot reaches a dead end, the robot will initially start turning left (evaluate the else statement) and detect that there is a wall in front of it. It will then reverse the turn and turn 180 degrees to continue to maze.

Due to the extra amount of complexity that arose from wall detection and in order to make our line following system more robust, we incorporated a finite state machine design for navigation. We start our robot in a "move forward" state that iteratively checks our left and right line sensors. If any of those sensors detect that we are no longer line following, then we branch into their respective states to adjust accordingly. If any of the adjusting or "move forward" states also detect that we are at an intersection, by checking if both the left and right sensors are on white, and need to turn according to the right hand rule logic above, we branch into a state that moves the robot slightly past the intersection (forward_until_past_intersection) and sets the turn diction by setting a variable called turn_direction. If we do need to turn, we branch into a state that performs the turn based on the turn_direction (start_turn), determine that the turn is completed (wait_until_turn_completed) based on the line sensors, and then branches back into the "move forward" state and continue our maze detection.

Excerpts from the code are as follows. The "only_left_on_white" and "only_right_on_white" function check if only one of the line sensors is on white based on empirical thresholds from the previous lab. We also created "left_on_white" and "right_on_white" to check if the respective sensor is on white. The "both_on_white" function checks if both the line sensors are on white. The "turn_status[turn_direction]" returns "left_on_white" for a left turn and "right_on_white" for a right turn as we start turning left when the left line sensor is no longer on white and turn right when the right line sensor is no longer on white.

```
int move_forward() {
  Serial.println("move forward");
  front_led_off(); // initialize the leds to not detect a wall
  right_led_off();
  set_left(1); // move the robot forward
  set_right(1);
  if (only_left_on_white()) return ADJUST_LEFT; // branch into adjust left state
  if (only_right_on_white()) return ADJUST_RIGHT; // branch into adjust right state 
  if (both_on_white() && (front_wall() || !right_wall() || opticalFFT())) {
  // check if we are at an intersection and if there is a front wall, no right wall, or a decoy robot
  // in which case we need to turn
    return FORWARD_UNTIL_PAST_INTERSECTION; // at an intersection and need to turn
  }
  return MOVE_FORWARD;
}

int forward_until_past_intersection() { // must turn after this state
  Serial.println("forward_until_past_intersection");
  set_left(1); // move forward
  set_right(1);
  if (both_on_white()) return FORWARD_UNTIL_PAST_INTERSECTION; // move a little past the intersection
  if (!right_wall()) turn_direction = 1; // turn right if no right wall
  else turn_direction = 0; // else turn left
  if (!turn_status[turn_direction]()) {
    // turn_status[turn_direction() checks if left or right line sensor is on white
    // and so we turn when the respective sensor is past the intersection. We stop, delay, and turn.
    set_left(0); 
    set_right(0);
    delay(10);
    return START_TURN;
  }
  return FORWARD_UNTIL_PAST_INTERSECTION;
}

int start_turn() {
  Serial.println("start turn");
  set_left(turn_direction); // set the left and white wheels based on the turn direction
  set_right(1 - turn_direction); 
  if (!turn_status[turn_direction]()) return START_TURN;
  if (turn_status[turn_direction]()) {
    // if we have seen the respective sensor on white thus halfway done with turn
    // But before completing turn, check for a dead end.
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
  // turn_status[turn_direction] checks if either the left or right line sensor is on white.
  // we continue turning until respective sensor is no longer on white line
  if (turn_status[turn_direction]()) return WAIT_UNTIL_TURN_END;
  if (!turn_status[turn_direction]()) return MOVE_FORWARD;
  return WAIT_UNTIL_TURN_END;
}


```

Once we developed these states and branching conditions, wall-detection worked. The following video demonstrates how our robot is able to detect walls and line follows. Note the flashing LEDs.

# https://youtu.be/sdR-A3kpq3s #


## Avoiding other Robots

In order to avoid other robots, we attached an IR Sensor to the front of the robot. The robot performs an FFT utilizing the "optical_FFT" function written in Lab 2. If we do detect a decoy robot, then we treat it as a wall in our wall detection algorithm above. This is why we added an OR statement with "optical_FFT()" above in the "move_forward" state as well as added similar conditions in the "adjust_left" and "adjust_right" states. We initially had issues with integrating the FFT but we adjusted our value of ADCSRA and we found it to work.

A video of our robot avoiding other robots, walls, and line following is as follows:

# https://youtu.be/XYCFWvGQjow #
