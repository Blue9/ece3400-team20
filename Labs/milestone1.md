***Milestone 1***

*Lab Procedure:*

*Objective:* The objective of this milestone is to be able to add sensors to the robot as well as program the robot such that the robot will be able to line follow and turn along a grid in a figure eight fashion.

We used two infrared sensors with spacing slightly greater than the tape’s width placed in front of the robot approximately 5 mm above the floor. This placement was chosen so that when the robot detects an intersection both line sensors would read white and while the robot is going straight along a path, both line sensors would read black. If any of the line sensors read white exclusively, i.e. the left sensor reads white and the right sensor reads black or vice-versa, we know that the robot is angled against the line and should adjust accordingly. This adjustment forms the basis  for our line-following procedure. We then calibrated the threshold value of each sensor by reading the value on the analog pins along lighter and darker regions of the grid and averaged accordingly. 

*Loop Iteration States:*

To implement line following and turning, on each loop iteration, we checked the value of the line sensors with respect to the threshold and classified it in the following four states:
	
Left on Right off: If the left sensor reads white and the right sensor reads black, then we know that the robot is angled against the line, i.e. it has turned too far to the right while line following. As a result, we adjust accordingly by stopping the right wheel (turn left) until both line sensors detect black. We then continue both wheels full speed ahead from there.	

```
adjust_left(0);
while (sensor_on_white(LEFT_COLOR_PIN) && !sensor_on_white(RIGHT_COLOR_PIN)) {
// Wait until neither on white.
}
adjust_left(MAX_SPEED);
```		

Left off right on: If the right sensor reads white and the right sensor reads black, then we know that the robot is angled against the line in the other direction, i.e. it has turned too far to the left while line following. As a result, we adjust accordingly by stopping the left wheel (turn right) until both line sensors detect black. We then continue both wheels full speed ahead from there.

adjust_right(0);
while (sensor_on_white(RIGHT_COLOR_PIN) && !sensor_on_white(LEFT_COLOR_PIN)) {
// Wait until neither on white.
}
adjust_right(MAX_SPEED);


Both On:  Based off of the sensor alignment we chose, as aforementioned, if both sensors are on, we must be at an intersection. In order to complete a figure eight in the configuration we chose, the robot must turn left four times and then turn right four times. We kept a variable that kept track of which turn number we are on and used the mod function to decide whether to turn left or right. For example, if the counter variable records the 9th turn, the robot must be at an equivalent position as for the 1st turn (9 % 8 = 1) and turn left. Once we determined which way to turn, we called our turning functions accordingly.
		


// If both the right and left sensors are on white,
// then we are at an intersection.
if (figure_eight_step % 8 > 3) {
Serial.println("Turning left");
turn_left();
} else {
Serial.println("Turning right");
turn_right();
}
figure_eight_step = (figure_eight_step + 1) % 8;



Both off: If both sensors are off, then the robot is aligned with the tape.  In this case, we continue forward and have both wheels go at their full speeds. 

// Move straight.
adjust_left(MAX_SPEED);
adjust_right(MAX_SPEED);



*Turning Procedures*

As aforementioned, since the sensors are slightly farther apart then the line is wide, when both sensors detect a line, we know that an intersection has been found. We developed functions to turn in each direction: turn_left() and turn_right(). The function turn_left() stops the left wheel, sets the right wheel to full speed, delays for 900 msec, and restarts the left wheel. The function turn_right() does the same with the wheels switched. We arrived on the timing of 900 msec through testing. If the turn is not exact, the line following algorithm will ensure the robot returns to being centered on the line for increased robustness.

/**
 * Turn the robot left 90 degrees by slowing down the left servo.
 */
void turn_left() {
  adjust_left(0);
  adjust_right(1);
  delay(900);
  adjust_left(1);
}

/**
 * Turn the robot right 90 degrees by slowing down the right servo.
 */
void turn_right() {
  adjust_left(1);
  adjust_right(0);
  delay(900);
  adjust_right(1);
}

*Adjust Functions:*